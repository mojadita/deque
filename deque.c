/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $
 * Author : Luis Colorado <luiscoloradourcola@gmail.com>
 * Date   : Sat Sep 10 16:35:54 EEST 2016
 * Project: Services
 *
 * Disclaimer:
 * IRC-Hispano -- deque.c
 *
 * Copyright (C) 1998-2016 IRC-Hispano Development Team <devel@irc-hispano.es>
 * Copyright (C) 2016 Luis Colorado - mojadita <luiscoloradourcola@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define IN_DEQUE_C

/* Standard include files */
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "deque.h"

typedef struct deque_node_s {
    void           *dn_msg;
    LNODE_T         dn_node;
} DEQUE_NODE_T, *DEQUE_NODE_P;

/* functions */

void dq_init(DEQUE_P queue, int flags, size_t maxSize)
{
    queue->dq_flags = flags;
    LIST_INIT(&queue->dq_list);
    queue->dq_max = maxSize;
    pthread_mutex_init(&queue->dq_lock, NULL);
    pthread_cond_init(&queue->dq_not_empty, NULL);
    pthread_cond_init(&queue->dq_not_full, NULL);
} /* dq_init */

static void dq_common(DEQUE_P queue, void *theMessage, int isInsert)
{
    DEQUE_NODE_P theNode = malloc(sizeof *theNode);
    assert(theNode != NULL);

    theNode->dn_msg = theMessage;

    /* acquire the mutex */
    pthread_mutex_lock(&queue->dq_lock);

    /* wait for the queue not be full */
    if (    queue->dq_max
         && queue->dq_list.l_size >= queue->dq_max)
    {
        pthread_cond_wait(&queue->dq_not_full,
                &queue->dq_lock);
    }
    /* queue->dq_max == 0 || queue->dq_node.size < queue->dq_max */

    /* append the element to the end of the queue */
    if (isInsert) {
        LIST_INSERT_ELEMENT(&queue->dq_list,
                theNode, DEQUE_NODE_T, dn_node);
    } else {
        LIST_APPEND_ELEMENT(&queue->dq_list,
                theNode, DEQUE_NODE_T, dn_node);
    }

    /* signal we are not empty */
    pthread_cond_signal(&queue->dq_not_empty);

    /* unlock the mutex */
    pthread_mutex_unlock(&queue->dq_lock);
} /* dq_common */

void dq_ins(DEQUE_P queue, void *theMessage)
{
    dq_common(queue, theMessage, 1);
} /* dq_ins */

void dq_add(DEQUE_P queue, void *theMessage)
{
    dq_common(queue, theMessage, 0);
} /* dq_add */

void *dq_get(DEQUE_P queue)
{
    return dq_timedget(queue, 0);
} /* dq_get */

void *dq_timedget(DEQUE_P queue, struct timespec *whenAbsTO)
{
    DEQUE_NODE_P theNode = NULL;
    void *theMessage = NULL;
    int res = 0;

    /* acquire the mutex */
    pthread_mutex_lock(&queue->dq_lock);

    /* wait for the queue not be empty */
    if (queue->dq_list.l_size <= 0) {
        res = (whenAbsTO != NULL)
            ? pthread_cond_timedwait(&queue->dq_not_empty,
                    &queue->dq_lock, whenAbsTO)
            : pthread_cond_wait(&queue->dq_not_empty,
                    &queue->dq_lock);
    } /* while */

    if (res == 0) { /* we got something */
        theNode = LIST_FIRST_ELEMENT(&queue->dq_list,
                DEQUE_NODE_T, dn_node);
        LIST_UNLINK_ELEMENT(&queue->dq_list, theNode,
                DEQUE_NODE_T, dn_node);
        pthread_cond_signal(&queue->dq_not_full);
    }

    pthread_mutex_unlock(&queue->dq_lock);

    if (theNode) {
        theMessage = theNode->dn_msg;
        free(theNode);
    }

    return theMessage;
} /* dq_timedget */

/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */

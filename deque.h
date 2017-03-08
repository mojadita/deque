/* $Id: header.h.m4,v 1.7 2005/11/07 19:39:53 luis Exp $
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Sat Sep 10 16:17:36 EEST 2016
 *
 * IRC-Hispano Services -- deque.h
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

/* Do not include anything BEFORE the line below, as it would not be
 * protected against double inclusion from other files
 */
#ifndef DEQUE_H
#define DEQUE_H

#include "./lists.h"
#include <pthread.h>

/* constants */
#define DQ_FLAG_DEBUG   (1 << 0)

/* types */

typedef struct deque_s {
    int             dq_flags;
    LLIST_T         dq_list;
    size_t          dq_max;
    pthread_mutex_t dq_lock;
    pthread_cond_t  dq_not_empty;
    pthread_cond_t  dq_not_full;
} DEQUE_T, *DEQUE_P;

/* prototypes */

extern void dq_init(DEQUE_P queue, int flags, size_t maxSize);
extern void dq_add(DEQUE_P queue, void *msg);
extern void dq_ins(DEQUE_P queue, void *msg);
void *dq_timedget(DEQUE_P queue, struct timespec *whenAbsTO);
void *dq_get(DEQUE_P queue);

#endif /* DEQUE_H */
/* Do not include anything AFTER the line above, as it would not be
 * protected against double inclusion from other files.
 */
/* $Id: header.h.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */

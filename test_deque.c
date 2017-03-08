/* thread safe double ended queues to implement message queues.
 * Author: Luis Colorado <luiscoloradourcola@gmail.com>
 * Date: Thu Sep 29 18:03:43 CEST 2016
 * Project: Services
 * Customer: CHATHISPANO
 * Copyright: (C) 2016 LUIS COLORADO.
 *
 * This program is open source as required by the BSD Open Source
 * license.  You are free to copy/distribute/modify this program,
 * while you respect the above copyright note.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <getopt.h>

#include "deque.h"

#define OPT_DEBUG   (1 << 0)

#define MIN 1
#define MAX 10

int n       = 1;
int N       = 2;
int NUM     = 5;
int flags   = 0;

struct theThread {
    int                 id;
    struct theThread  **to;
    int                 to_n;
    int                 i;
    int                 n_eofs;
    DEQUE_T             queue;
    pthread_t           thread;
};

#define F(x) "%*s[%02d] " x, 2*th->id, "", th->id
void *thread_routine(void *arg)
{
    struct theThread *th = arg;
    char *msg;
    int i;
    printf(F("BEGIN (%d EOFs to go)\n"), th->n_eofs);fflush(stdout);
    for(i = 0; i < th->n_eofs; i++) {
        while((msg = dq_get(&th->queue))) {
            if (th->to_n) {
                printf(F("%s [%02d]\n"), msg, th->to[th->i]->id);fflush(stdout);
                dq_add(&th->to[th->i]->queue, msg);

                th->i++; if (th->i >= th->to_n) th->i = 0;
            } else {
                printf(F("%s NUL\n"), msg); fflush(stdout);
                free(msg); /* no more journey for this message */
            }
        }
        printf(F("EOF(%d/%d)\n"), i+1, th->n_eofs);
    }
    for (i = 0; i < th->to_n; i++) {
        printf(F("EOF(%d/%d) [%02d]\n"), i+1, th->to_n, th->to[i]->id);fflush(stdout);
        dq_add(&th->to[i]->queue, NULL);
    }
    printf(F("END\n"));fflush(stdout);
    return NULL;
}   

int main(int argc, char **argv)
{
    struct theThread *array;
    int i, opt;

    while((opt = getopt(argc, argv, "vn:N:")) != EOF) {
        switch(opt) {
        case 'v': flags |= OPT_DEBUG; break;
        case 'n': n = atoi(optarg);
                  if (n > MAX) n = MAX;
                  if (n < MIN) n = MIN;
                  break;
        case 'N': NUM = atoi(optarg);
                  if (NUM < 1) NUM = 1;
                  break;
        } /* switch */
    } /* while */

    N = (1 << n); 

    array = calloc(N, sizeof *array);
    assert(array != NULL);

    for (i = 0; i < N; i++) {
        int j;
        int count = 0;

        array[i].id = i;
        array[i].i = 0;
        array[i].to = malloc(n * sizeof (struct theThread *));
        for (j = 1; j < N; j <<= 1) {
            int k = (j | i);
            if (k > i) {
                array[i].to[count++] = array + k;
                array[k].n_eofs++;
            }
        }
        array[i].to_n = count;
        dq_init(&array[i].queue,
                flags & OPT_DEBUG
                    ? DQ_FLAG_DEBUG
                    : 0,
                0);
    }
    array[0].n_eofs++;
    for (i = 0; i < N; i++) {
        int res = pthread_create(&array[i].thread, NULL,
                thread_routine, array + i);
        if (res) {
            fprintf(stderr, "pthread_create: %s(%d)",
                    strerror(errno), errno);
            exit(1);
        }
    }
    for (i = 0; i < NUM; i++) {
        char buff[128];
        snprintf(buff, sizeof buff,
                "<M-%02d>", i);
        dq_add(&array[0].queue, strdup(buff));
    }
    dq_add(&array[0].queue, NULL);
    pthread_join(array[N-1].thread, NULL);
    return 0;
} /* main */

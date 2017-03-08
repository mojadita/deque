/* $Id: lists.h.in,v 1.4 2007/04/25 22:35:33 luis Exp $
 * Author: Luis Colorado <lc@luiscoloradosistemas.com>
 * Date: Mon Jul 18 17:21:22 CEST 2005
 *
 * Disclaimer:
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Do not include anything BEFORE the line below, as it would not be
 * protected against double inclusion from other files
 */
#ifndef LISTS_H
#define LISTS_H

#include <assert.h>

#define LISTS_H_PACKAGE_NAME "lists"
#define LISTS_H_PACKAGE_VERSION "1.0"
#define LISTS_H_PACKAGE_STRING "lists 1.0"

/* constants */

/* types */
typedef struct LNODE_S {
		struct LNODE_S *n_prev;
		struct LNODE_S *n_next;
} LNODE_T, *LNODE_P;

typedef struct LIST_S {
    size_t  l_size;
    LNODE_P l_first;
    LNODE_P l_last;
} LLIST_T, *LLIST_P;

/* definitions */

#ifndef OFFSETOF
        /* gives the offset of field f in type T */
#define OFFSETOF(T,f) ((char*)&(((T*)0)->f)-(char*)0)
#endif

        /* initialize fields of list L */
#define LIST_INIT(L) do{\
        (L)->l_size=0;\
        (L)->l_first=(L)->l_last=0;\
    }while(0)

#define LIST_DECLARE(L) \
    LLIST_T L={\
        .l_size=0,\
        .l_first=0,\
        .l_last=0\
    }

#define LIST_FIRST(L) ((L)->l_first)
#define LIST_LAST(L) ((L)->l_last)
#define LIST_NEXT(N) ((N)->n_next)
#define LIST_PREV(N) ((N)->n_prev)

#define LIST_NODE(E,F)      ((LNODE_P)&(E)->F)
#define LIST_ELEMENT(N,T,F) ((N)?(T*)((char*)(N)-OFFSETOF(T,F)):0)

#define LIST_FIRST_ELEMENT(L,T,F) (LIST_ELEMENT(LIST_FIRST(L),T,F))
#define LIST_LAST_ELEMENT(L,T,F) (LIST_ELEMENT(LIST_LAST(L),T,F))
#define LIST_NEXT_ELEMENT(E,T,F) (LIST_ELEMENT(LIST_NEXT(LIST_NODE(E,F)),T,F))
#define LIST_PREV_ELEMENT(E,T,f) (LIST_ELEMENT(LIST_PREV(LIST_NODE(E,F)),T,F))

#define LIST_FOREACH(p, L) for(p=LIST_FIRST(L);p;p=LIST_NEXT(p))
#define LIST_FORBACK(p, L) for(p=LIST_LAST(L);p;p=LIST_PREV(p))
#define LIST_FOREACH_ELEMENT(p,L,T,F) for(p=LIST_FIRST_ELEMENT(L,T,F);p;p=LIST_NEXT_ELEMENT(p,T,F))
#define LIST_FORBACK_ELEMENT(p,L,T,F) for(p=LIST_LAST_ELEMENT(L,T,F);p;p=LIST_PREV_ELEMENT(p,T,F))

#define LIST_EMPTY(L) (!(L)->l_size)

#define __LIST_COMMON_INSERT(L,N,fst,lst,nxt,prv) do{   \
        (N)->nxt = (L)->fst;                            \
        (N)->prv = 0;                                   \
        if ((L)->fst)                                   \
            (L)->fst->prv=(N);                          \
        else                                            \
            (L)->lst=(N);                               \
        (L)->fst=(N);                                   \
        (L)->l_size++;                                  \
    }while(0)
#define LIST_INSERT(L,N) __LIST_COMMON_INSERT(L,N,l_first,l_last,n_next,n_prev)
#define LIST_APPEND(L,N) __LIST_COMMON_INSERT(L,N,l_last,l_first,n_prev,n_next)

#define __LIST_COMMON_UNLINK(L,N,lst,nxt,prv)           \
        if ((N)->nxt) (N)->nxt->prv = (N)->prv;         \
        else {                                          \
            assert((L)->lst == (N));                    \
            (L)->lst = (N)->prv;                        \
        }

#define LIST_UNLINK(L,N) do{                            \
        assert((L)->l_size);                            \
        (L)->l_size--;                                  \
        __LIST_COMMON_UNLINK(L,N,l_last,n_next,n_prev); \
        __LIST_COMMON_UNLINK(L,N,l_first,n_prev,n_next);\
        (N)->n_next = (N)->n_prev = 0;                  \
    } while(0)

#define LIST_INSERT_ELEMENT(L,E,T,F) LIST_INSERT(L,LIST_NODE(E,F))
#define LIST_APPEND_ELEMENT(L,E,T,F) LIST_APPEND(L,LIST_NODE(E,F))
#define LIST_UNLINK_ELEMENT(L,E,T,F) LIST_UNLINK(L,LIST_NODE(E,F))

#endif /* LISTS_H */
/* Do not include anything AFTER the line above, as it would not be
 * protected against double inclusion from other files.
 */
/* $Id: lists.h.in,v 1.4 2007/04/25 22:35:33 luis Exp $ */

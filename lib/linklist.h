/*  linklist.h

MIT License

Copyright (c) 2019 Daniel Koblas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef LINKLIST_H
#define LINKLIST_H

/* this link list supports the following operations: 
append, push, pop, pop_tail, remove, peek.  see prototypes below for syntax.

each node stores the data payload as a void pointer.

list usage:
set up your list variable where you want to use it, then pass the pointer to 
the init function, i.e.:

    link_list my_list;
    init_link_list(&my_list);

likewise pass a pointer to the data you want store/remove:

    int my_data = 7;
    list_append(&my_list, &my_data);
    list_remove(&my_list, &my_data);

methods that remove data return a pointer to the data stored there.  for struct
you get a pointer to the struct, but for other data types, you will need to 
dereference this pointer to do a direct assignment. for something like
an integer you have to cast it to the right kind of pointer first:

    int my_data;
    my_data = *(int*) list_pop(&my_list);

this linklist is designed to not use malloc, so we have a table of node structs 
that belongs to each list.  these are initialized to a usable state when we 
pass the list to init_link_list.  the number of nodes each list has avaialbe is
defined in the MAX_LIST_NODES macro below.
 */

// this should be at least 3 or else what is the point...
#define MAX_LIST_NODES 100

/* ----- data structure/types ----- */

typedef struct list_node {

    int index;
    void* payload;

    // next and prev node pointers
    struct list_node* next_node;
    struct list_node* prev_node;

} list_node;

typedef struct link_list { 

    struct link_list* self_ptr; // pointer to itself

    list_node nodes[MAX_LIST_NODES];

    // head and tail node pointers
    list_node* head_node;
    list_node* tail_node;

    /*  list of available nodes.  if this list is null then the list is full
        this is a strict FIFO queue and is not doubly linked.
        all nodes on this list should have prev = null */
    list_node* available_nodes;
    list_node* available_tail;

    int size; // max size of the list
    int length; // current number of nodes in the list.

} link_list;


/* ----- prototypes ----- */

// initialize list
int init_link_list(link_list* list);

// add node to end/tail of list
int list_append(link_list* list, void* payload);
// remove & return node from any position in list
void* list_remove(link_list* list, void* target);
// add node to the front/head of the list
int list_push(link_list* list, void* payload);
// remove & return node at front/head of the list
void* list_pop(link_list* list);
// remove & return node at end/tail of the list
void* list_pop_tail(link_list* list);
// return front/head of list without removing
void* list_peek(link_list* list);

#endif
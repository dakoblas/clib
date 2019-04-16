/* linklist.c

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


see linklist.h for usage and notes.
*/
#include <stdio.h> // for null macro
#include "linklist.h"

/* for list interal use only */
list_node* deq_new_node(link_list* list);
void enq_new_node(link_list* list, list_node* node);

/* new_list
Serves as "constructor"/initializer for new linked list
param: pointer to list
return: 0 for success
*/
int init_link_list(link_list* list) {

    // set self pointer
    list->self_ptr = list;

    /* initial state of the list nodes is that all nodes are linked together
    on the "available_nodes" queue.  using available_nodes, we can get the next 
    open node in 1 op.  nodes will be popped from this list when put into use
    and pushed onto the list when freed. */

    // initialize all nodes in the middle
    for (int i = 0; i < MAX_LIST_NODES - 1; i++) {
        list->nodes[i].index = i;
        list->nodes[i].payload = NULL;
        list->nodes[i].next_node = &list->nodes[i + 1];
        list->nodes[i].prev_node = NULL;
    }

    // initialize the last node
    list->nodes[MAX_LIST_NODES - 1].index = MAX_LIST_NODES - 1;
    list->nodes[MAX_LIST_NODES - 1].payload = NULL;
    // as last node the next pointer is null
    list->nodes[MAX_LIST_NODES - 1].next_node = NULL;
    list->nodes[MAX_LIST_NODES - 1].prev_node = NULL;

    // set the first node as the node pointed to by the available_nodes queue
    list->available_nodes = &list->nodes[0];
    // set the available tail pointer to the last node
    list->available_tail = &list->nodes[MAX_LIST_NODES - 1];

    // initialize the head and tail pointers
    list->head_node = NULL;
    list->tail_node = NULL;

    // set list max size
    list->size = MAX_LIST_NODES;

    // set initial length count
    list->length = 0;

    // set self pointer
    list->self_ptr = list;

    return 0;

}

/*  deq_new_node
    returns pointer to new node or null if none are available (list is full)
*/
list_node* deq_new_node(link_list* list) {

    list_node* ret_ptr = NULL;

    if (list->available_nodes == NULL) {
        return ret_ptr;
    }

    // get link to next new node
    ret_ptr = list->available_nodes;
    // update list's queue to the next one in line
    list->available_nodes = list->available_nodes->next_node;
    // unlink the new node
    ret_ptr->next_node = NULL;

    return ret_ptr;
} /* deq_new_node */

void enq_new_node(link_list* list, list_node* node) {

    // clear payload
    node->payload = NULL;
    // clear links
    node->next_node = NULL;
    node->prev_node = NULL;
    if (list->available_tail == NULL) {
        list->available_tail = node;
    }
    else {
        // enqueue node into available nodes list
        list->available_tail->next_node = node;
        // update list available tail
        list->available_tail = node;
    }

    return;
} /* enq_new_node */

/*  list_append
    add new element to the end of the list and increment the length counter
    return 0 for success and -1 for failure
*/
int list_append(link_list* list, void* payload) {
    
    // get next available node to use
    list_node* new = deq_new_node(list);
    
    // check for problem
    if (new == NULL) {
        return -1;
    }

    // printf("list_append: adding payload %p to node\n", payload);
    // add payload to node
    new->payload = payload;

    // printf("list_append: node payload is %p\n",new->payload);

    // update list length
    list->length++;

    // if this is the first node, we set to head and tail and we're done
    if (list->head_node == NULL) {
        // printf("list_append: this is first node in list\n");
        list->head_node = new;
        list->tail_node = new;
        return 0;
    }

    // link node to end of the other nodes
    // set new node's prev pointer to current tail
    new->prev_node = list->tail_node;
    // set current tail's next to new node
    list->tail_node->next_node = new;
    // update list's tail to new node
    list->tail_node = new;

    // printf("list_append: list length now %d\n", list->length);

    return 0;

} /* list_append */

/*  list_pop
    remove node from head of linked list and return pointer to its payload
    returns void* to payload data (or NULL if there was no node)
*/
void* list_pop(link_list* list) {

    void* ret_ptr = NULL;

    // if there are no nodes, return null
    if (list->head_node == NULL) {
        return NULL;
    }

    // get the head node
    list_node* node = list->head_node;

    // get the payload
    ret_ptr = node->payload;
    // reset the node's payload
    node->payload = NULL;

    // update list head to next node
    list->head_node = node->next_node;

    // update next's prev pointer (if null, update list tail pointer)
    if (node->next_node == NULL) {
        list->tail_node = list->head_node;
    }
    else {
        node->next_node->prev_node = NULL;
    }

    // unlink node from list
    node->next_node = NULL;

    // enqueue node into available nodes list
    enq_new_node(list, node);

    list->length--;

    // return the payload pointer
    return ret_ptr;
}

/*  list_remove
    searches list for a target payload, then removes and returns
    the pointer to the target payload from the node (or null if not present)
*/
void* list_remove(link_list* list, void* target) {

    // printf("list_remove: passed target %p\n", target);

    void* ret_ptr = NULL;

    // if there are no nodes, return null
    if (list->head_node == NULL) {
        return NULL;
    }

    // node will store the node containing target, if found
    list_node* target_node = NULL;

    // current will keep track of where we are in the list
    list_node* current_node = list->head_node;

    int i = 0;
    int found = 0;
    while (!found && i < list->length && current_node != NULL) {
        // printf("list_remove: this node payload is %p\n", current_node->payload);
        if (current_node->payload == target) {
            found = 1;
            target_node = current_node;
        }
        current_node = current_node->next_node;
        i++;
    }

    if (!found) {
        // printf("list_remove: target node not found.\n");
        return NULL;
    }

    ret_ptr = target_node->payload;
    target_node->payload = NULL;

    // unlink target node from the list
    // if prev exists, set its next to our next
    if (target_node->prev_node != NULL) {
        target_node->prev_node->next_node = target_node->next_node;
    }
    else {
        // if prev is null then this is last node, update head pointer.
        list->head_node = target_node->next_node;
    }
    // if next exists, set its prev to our prev
    if (target_node->next_node != NULL) {
        target_node->next_node->prev_node = target_node->prev_node;
    }
    else {
        // if next is null then this is last node and need to update tail pointer
        list->tail_node = target_node->prev_node;
    }

    // reset target node links
    target_node->next_node = NULL;
    target_node->prev_node = NULL;
    
    // recycle target node
    enq_new_node(list, target_node);

    list->length--;
    // printf("list_remove: list length now %d\n", list->length);

    return ret_ptr;
}

/*  list_pop_tail
    remove node from tail end of linked list and return pointer to its payload
    returns void* to payload data (or NULL if there was no node)
*/
void* list_pop_tail(link_list* list) {

    void* ret_ptr = NULL;

    // if there are no nodes, return null
    if (list->tail_node == NULL) {
        return NULL;
    }

    // get the head node
    list_node* node = list->tail_node;

    // get the payload
    ret_ptr = node->payload;
    // reset the node's payload
    node->payload = NULL;

    // update list head to next node
    list->tail_node = node->prev_node;

    // update prev's next pointer (if null, update list head pointer)
    if (node->prev_node == NULL) {
        list->head_node = list->tail_node;
    }
    else {
        node->prev_node->next_node = NULL;
    }

    // unlink node from list
    node->prev_node = NULL;

    // enqueue node into available nodes list
    enq_new_node(list, node);

    list->length--;

    // return the payload pointer
    return ret_ptr;
}

/*  list_push
    add a node to the head of the list
    return 0 for success and -1 for fail
*/
int list_push(link_list* list, void* payload) {

    // get next available node to use
    list_node* new = deq_new_node(list);
    
    // check for problem
    if (new == NULL) {
        return -1;
    }

    // add payload to node
    new->payload = payload;

    // update list length
    list->length++;

    // get the head node
    list_node* node = list->head_node;

    // if null then this is the first node 
    if(node == NULL) {
        list->head_node = new;
        list->tail_node = new;
        return 0;
    }

    // if there were other nodes already, insert to the front
    // set the current head node to be new node's next
    new->next_node = list->head_node;
    // set that node's prev ptr to this new node
    new->next_node->prev_node = new;
    // set new to the new head
    list->head_node = new;

    return 0;
}

/*  list_peek
    return data contained in head node without removing from list
*/
void* list_peek(link_list* list) {
    if (list->head_node == NULL) {
        return NULL;
    }
    return list->head_node->payload;
}
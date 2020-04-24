#include <atomic>
#include <stdio.h>
#include <stdlib.h>
#include "deque_operations.h"

using namespace std;

//Initialize node
void init_deque_node(atomic_deque_node_t &node, int *init_null) {
    deque_node_t blank_node;
    blank_node.value = init_null;
    blank_node.count = 0;
    node.store(blank_node);
}

//Initialize queue
void init_deque(deque_t &deque) {
    int i;
    
    for(i = 0; i < DEF_BOUNDS; i++) {
        if(i < DEF_BOUNDS / 2)
            init_deque_node(deque.nodes[i], LNULL);
        else 
            init_deque_node(deque.nodes[i], RNULL);
    }
    
    deque.size = 0;
    deque.left = DEF_BOUNDS / 2 - 1;
    deque.right = DEF_BOUNDS / 2;
}

//Clear node 
void clear_deque_node(atomic_deque_node_t &node) {
    deque_node_t blank_node, old_node;
    blank_node.value = NULL;
    blank_node.count = 0;
    old_node = node.exchange(blank_node);
}

//Clear queue
void clear_deque(deque_t &deque) {
    int i;
    
    for(i = 0; i < DEF_BOUNDS; i++) {
        if(i < DEF_BOUNDS / 2)
            init_deque_node(deque.nodes[i], LNULL);
        else 
            init_deque_node(deque.nodes[i], RNULL);
    }
    
    deque.size = 0;
    deque.left = DEF_BOUNDS / 2 - 1;
    deque.right = DEF_BOUNDS / 2;
}

//Set value and counter of node
void set_deque_node(deque_node_t &node, int *value, unsigned int last_count) {
    node.value = value;
    node.count = last_count + 1;
}

void copy_deque_node(deque_node_t &new_node, deque_node_t &old_node) {
    new_node.value = old_node.value;
    new_node.count = old_node.count + 1;
}

//Left push op
void left_push(deque_t &deque, int *elt, int &stat) {
    unsigned long int k;
    deque_node_t previous, current;
    
    while(1) {
        k = oracle(deque, LEFT);
        //k = deque.left.load();
        previous = deque.nodes[k + 1].load();
        current = deque.nodes[k].load();
        
        if(previous.value != LNULL && current.value == LNULL) {
            if(k <= 0) {
                stat = FULL;
                fprintf(stdout, "\nQueue is full\n");
                return;
            }

            deque_node_t prev_new, cur_new;
            copy_deque_node(prev_new, previous);
            set_deque_node(cur_new, elt, current.count);
            // fprintf(stdout, "\nPushed element(left) to Queue\n");

            if(deque.nodes[k + 1].compare_exchange_strong(previous, prev_new)) {
                if(deque.nodes[k].compare_exchange_strong(current, cur_new)) {
                    // update loc hint
                    deque.left--;
                    deque.size++;
                    stat = OK;
                    return;
                }
            }
        }
    }
}

//Left pop op
int *left_pop(deque_t &deque, int &stat) {
    unsigned long int k;
    deque_node_t current, next;
    
    while(1) {
        k = oracle(deque, LEFT);
        //k = deque.left.load();
        current = deque.nodes[k + 1].load();
        next = deque.nodes[k].load();
        
        if(current.value != LNULL && next.value == LNULL) {
            if(current.value == RNULL && compare_val(deque.nodes[k + 1], current)) {
                stat = EMPTY;
                fprintf(stdout, "\nQueue is empty\n");
                return NULL;
            }
            
            deque_node_t cur_new, next_new;
            set_deque_node(cur_new, LNULL, current.count);
            set_deque_node(next_new, LNULL, next.count);
            // fprintf(stdout, "\nPopped element(left) from Queue\n");
            if(deque.nodes[k].compare_exchange_strong(next, next_new)) {
                if(deque.nodes[k + 1].compare_exchange_strong(current, cur_new)) {
                    // update loc hint
                    deque.left++;
                    deque.size--;
                    stat = OK;
                    return current.value;
                }
            }
        }
    }
}

//Right push pop
void right_push(deque_t &deque, int *elt, int &stat) {
    unsigned long int k;
    deque_node_t previous, current;
    
    while(1) {
        k = oracle(deque, RIGHT);
        //k = deque.right.load();
        previous = deque.nodes[k - 1].load();
        current = deque.nodes[k].load();
        
        if(previous.value != RNULL && current.value == RNULL) {
            if(k >= DEF_BOUNDS - 1) {
                stat = FULL;
                fprintf(stdout, "\nQueue is full\n");
                return;
            }

            deque_node_t prev_new, cur_new;
            copy_deque_node(prev_new, previous);
            set_deque_node(cur_new, elt, current.count);
            // fprintf(stdout, "\nPushed element(right) to Queue\n");
            if(deque.nodes[k - 1].compare_exchange_strong(previous, prev_new)) {
                if(deque.nodes[k].compare_exchange_strong(current, cur_new)) {
                    // update loc hint
                    deque.right++;
                    deque.size++;
                    stat = OK;
                    return;
                }
            }
        }
    }
}

//Right pop op
int *right_pop(deque_t &deque, int &stat) {
    unsigned long int k;
    deque_node_t current, next;
    
    while(1) {
        k = oracle(deque, RIGHT);
        //k = deque.right.load();
        current = deque.nodes[k - 1].load();
        next = deque.nodes[k].load();
        
        if(current.value != RNULL && next.value == RNULL) {
            if(current.value == LNULL && compare_val(deque.nodes[k - 1], current)) {
                stat = EMPTY;
                fprintf(stdout, "\nQueue is empty\n");
                return NULL;
            }
            
            deque_node_t cur_new, next_new;
            set_deque_node(cur_new, RNULL, current.count);
            set_deque_node(next_new, RNULL, next.count);
            // fprintf(stdout, "\nPopped element(right) from Queue\n");
            if(deque.nodes[k].compare_exchange_strong(next, next_new)) {
                if(deque.nodes[k - 1].compare_exchange_strong(current, cur_new)) {
                    // update loc hint
                    deque.right--;
                    deque.size--;
                    stat = OK;
                    return current.value;
                }
            }
        }
    }
}

//Oracle functions
unsigned long int oracle(deque_t &deque, oracle_end deque_end) {
    unsigned long int i, k;
    deque_node_t current, previous, next;
    
    if(deque_end == LEFT) {
        k = deque.left.load();
        current = deque.nodes[k].load();
        previous = deque.nodes[k + 1].load();
        
        if(current.value == LNULL && previous.value != LNULL) {
            return k;
        } else if(current.value == LNULL && previous.value == LNULL) {
            // go right
            for(i = k; i < DEF_BOUNDS - 1; i++) {
                current = deque.nodes[i].load();
                previous = deque.nodes[i + 1].load();
                if(current.value == LNULL && previous.value != LNULL)
                    return i;
            }
            return deque.left.load();
        } else {
            return deque.left.load();
        }
    } else if(deque_end == RIGHT) {
        k = deque.right.load();
        current = deque.nodes[k].load();
        previous = deque.nodes[k - 1].load();
        
        if(current.value == RNULL && previous.value != RNULL) {
            return k;
        } else if(current.value == RNULL && previous.value == RNULL) {
            // go left
            for(i = k; i > 0; i--) {
                current = deque.nodes[i].load();
                previous = deque.nodes[i - 1].load();
                if(current.value == RNULL && previous.value != RNULL)
                    return i;
            }
            return deque.right.load();
        } else {
            return deque.right.load();
        }
    }
    else
        return 0;
}


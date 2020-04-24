#include <assert.h>
#include <atomic>
#include <stdio.h>
#include <stdlib.h>
// #include "deque_operations.h"
#include "deque_operations.cc"
using namespace std;

deque_t test_queue;

/*
 *     Tests:
 *     01 and 02. push one element, then pop it (left and right)
 *     03 and 04. push until full, the pop until empty (left and right)
 */

int test01(){
    fprintf(stdout, "\nTest 1");
    int push_status = 0, pop_status = 0;
    int push_value = 5, pop_value;
    left_push(test_queue, &push_value, push_status);
    assert(push_status == OK);
    assert(test_queue.size == 1);
    pop_value = *left_pop(test_queue, pop_status);
    assert(pop_status == 0);
    assert(test_queue.size == 0);
    assert(push_value == pop_value);
    return 0;
}

int test02() {
    fprintf(stdout, "\nTest 2");
    int push_status = 0, pop_status = 0;
    int push_value = 5, pop_value;
    right_push(test_queue, &push_value, push_status);
    assert(push_status == OK);
    assert(test_queue.size == 1);
    pop_value = *right_pop(test_queue, pop_status);
    assert(pop_status == 0);
    assert(test_queue.size == 0);
    assert(push_value == pop_value);
    return 0;
}

int test03() {
    fprintf(stdout, "\nTest 3");
    int i, e = 0, *push_value, push_status = 0, pop_status = 0, pop_value;
    unsigned long int half_cap = DEF_BOUNDS / 2 - 1;
    //fill to the left
    while(true) {
        if(test_queue.size >= half_cap)
            break;
        fprintf(stdout, "\nPush iteration %3d\n", e);
        push_value = (int *) malloc(sizeof(int));
        *push_value = e++;
        left_push(test_queue, push_value, push_status);
        assert(test_queue.size == e);
        assert(push_status == OK);
    }
    assert(test_queue.size == (DEF_BOUNDS / 2 - 1));
    //verify deque state
    fprintf(stdout, "\n\nVerify queue state\n");

    for(i = 1; i < DEF_BOUNDS - 1; i++) {
        fprintf(stdout, "\tCheck iteration %3d\n", i);
        if(i < DEF_BOUNDS / 2)
            assert(!is_null(test_queue.nodes[i].load().value));
        else
            assert(test_queue.nodes[i].load().value == RNULL);
    }
    //remove elements from the left
    while(true) {
        if(test_queue.size <= 0)
            break;
        fprintf(stdout, "\nPop iteration %3d\n", e);
        pop_value = *left_pop(test_queue, pop_status);
        assert(pop_value == --e);
        assert(test_queue.size == e);
        assert(pop_status == OK);
    }
    assert(test_queue.size == 0);
    //verify deque state
    fprintf(stdout, "\n\nVerify queue state\n");
    for(i = 1; i < DEF_BOUNDS - 1; i++) {
        fprintf(stdout, "\tCheck iteration %3d\n", i);
        if(i < DEF_BOUNDS / 2)
            assert(test_queue.nodes[i].load().value == LNULL);
        else
            assert(test_queue.nodes[i].load().value == RNULL);
    }
    return 0;
}

int main(int argc, char* argv[]){
    //Tests
    init_deque(test_queue);
    test01();
    clear_deque(test_queue);
    init_deque(test_queue);
    test02();
    clear_deque(test_queue);
    init_deque(test_queue);
    test03();
    clear_deque(test_queue);

    return 0;
}


#include <atomic>

#define LNULL (int *) 0b01
#define RNULL (int *) 0b10
#define DEF_BOUNDS 5500

using namespace std;

typedef struct deque_node_struct {
    int *value;
    unsigned int count;
} deque_node_t;

typedef atomic<deque_node_t> atomic_deque_node_t;

typedef struct deque_struct {
    atomic_deque_node_t nodes[DEF_BOUNDS];
    atomic_ulong size;
    atomic_ulong left;
    atomic_ulong right;
} deque_t;

typedef enum { LEFT, RIGHT } oracle_end;
typedef enum { OK, EMPTY, FULL } deque_state;

void left_push(deque_t &deque, int *elt, int &stat);
int *left_pop(deque_t &deque, int &stat);
void right_push(deque_t &deque, int *elt, int &stat);
int *right_pop(deque_t &deque, int &stat);
unsigned long int oracle(deque_t &deque, oracle_end deque_end);

void init_deque_node(atomic_deque_node_t &node);
void init_deque(deque_t &deque);
void clear_deque_node(deque_node_t &node);
void clear_deque(deque_t &deque);
void set_deque_node(deque_node_t &node, int *value, unsigned int last_count);
void copy_deque_node(deque_node_t &new_node, deque_node_t &old_node);

static inline bool compare_val(atomic_deque_node_t &a, deque_node_t &b) {
    deque_node_t a_copy = a.load();
    return a_copy.value == b.value;
}

static inline bool cas_node(atomic_deque_node_t &current, deque_node_t &expected, deque_node_t &desired) {
    return current.compare_exchange_strong(expected, desired);
}

static inline bool compare_node(atomic_deque_node_t &a, deque_node_t &b) {
    deque_node_t a_copy = a.load();
    return a_copy.value == b.value && a_copy.count == b.count;
}

static inline bool is_null(int *v) {
    return v == NULL || v == LNULL || v == RNULL;
}

static inline bool val_eql(deque_node_t &a, deque_node_t &b) {
    return a.value == b.value;
}




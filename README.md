# Parallel Data Structures

This library contains two parallel data structure implemenations; a lock-free exponential back-off stack, and a obstruction-free double-ended queue (deque). 

Also provided are two driver files for running multi threaded tests on each data structure; omptest_stack.cpp and omptest_deque.cpp.
## Lock-Free Exponential back-off stack
### To compile:
```
g++ -std=c++11 -latomic -fopenmp omptest_stack.cpp -o testStack
```

### To Execute:
```
./testStack [num_threads] [num_operations] [workload]
```
## Obstruction-free Double ended queue
### To compile:
```
g++ -std=c++11 -latomic -fopenmp omptest_deque.cpp -o testDeque
```

### To Execute:
```
./testDeque [num_threads] [num_operations] [workload]
```

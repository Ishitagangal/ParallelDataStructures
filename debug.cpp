//g++ -std=c++11 -latomic -fopenmp omptest.cpp -o testOmp
#include <iostream>
#include <stdio.h>
#include <chrono>
#include <thread>
// #include <omp.h> 
#include "deque_operations.cc"
using namespace std;

deque_t test_queue;

int main(int argc, char *argv[]){

  int thread_count = strtol(argv[1], NULL, 10);
  int num_ops = strtol(argv[2], NULL, 10);
  int workload = strtol(argv[3], NULL, 10);

  int pop_value, push_status = 0, pop_status = 0;
  int push_value;

  // partially fill the stack to enslsure no pops on while empty
  //Initialize deque
  init_deque(test_queue); 
    cout<<"Init";
  //Filling deque with left push operations
  for(int i = 0; i < num_ops/2; i++)
  {
    push_value = rand();
    cout<<push_value<<"\n";
    left_push(test_queue, &push_value, push_status);
  }
  
  // double start_time_tot = omp_get_wtime();
  double latency = 0;

  // #pragma omp parallel for num_threads(thread_count) reduction(+:latency)
  for(int i = 0; i < num_ops; i++){
    int op = rand() % 2;   // 50/50 chance for both push and pop

    // double start_time_lat = omp_get_wtime();
    //Perform a random left or right push operation
    if(op == 0 && push_status == OK)
    { 
      push_value = rand();
      //randomly either perform left push or right push operation 
      if(rand() % 2 == 0){
        left_push(test_queue, &push_value, push_status);
      }
      else
      {
        right_push(test_queue, &push_value, push_status);
      }
    } 
    else if(op == 1 && pop_status == OK)
    {
      //randomly either perform left pop or right pop operation 
      if(rand() % 2 == 0)
      {
        pop_value = *left_pop(test_queue, pop_status);
      }
      else
      {
        pop_value = *right_pop(test_queue, pop_status);
        cout<<"\n"<<pop_value;
      }    
    }
    else
    {
      fprintf(stdout, "\n\nQueue state is not OK to perform operation\n");
    }
    
    // latency += omp_get_wtime() - start_time_lat;
   
   // simulate program work flow by making threads wait for random intervals.
    if(workload > 0){
      int delay = rand() % workload + 1;  // unoiform distribution on [0, workload]; in milliseconds
      // std::this_thread::sleep_for(std::chrono::milliseconds(delay));    
    }
  } 

  // double total_time = omp_get_wtime() - start_time_tot;
  // printf("number of threads: %d, total_time(s): %lf\n", thread_count, total_time);
  // printf("throughput (ops per second): %lf\n", num_ops/total_time);
  printf("latency (avg time per op): %lf\n\n", latency/num_ops);

  //clear deque before program exit
  clear_deque(test_queue);
  return 0;
}



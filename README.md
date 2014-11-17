Parallel-CW1
============


1. **Environment:** 
  - Aquila, pthreads and C
2. **Objective:** 
  - give you experience of using low-level primitive parallelism constructs on a shared memory architecture
  - plus a feel for how parallel problems scale on such architectures
3. **Problem description:**
  - having an 2D array of values and repeatedly replacing a value with the average of its four neighbours
  - excepting boundary values, which remain at fixed values
  - repeating until all values settle down to within a given precision.
4. **Requirement:**
  - choice on what low-level primitives to use:
    - mutexes
    - barriers
    - semaphores
    - condition variables
    - pthread create
    - ...

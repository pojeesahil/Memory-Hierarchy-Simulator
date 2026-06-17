# Memory-Hierarchy-Simulator
I have built preemptive synchronous SRTF scheduler

Tasks will be added to task scheduler at interval of 1 cycle,
Task scheduler is implemented with minheap priority queue so automatically task with minimum burst time will be prioritised,

My scheduler is preemptive whenever new task with shorter burst time comes,current task is pushed backed to queue

I have Implemented single synchonorous core so when memory is requested, It stalls until the memory is fetched
However, for efficiency, I have built a cache system which first checks L1->L2->L3->RAM for memory, if memory is found in cache,it is fetched from cache otherwise data is fetched from RAM and also stored at L1,L2,L3 cache for future,This leads to lesser time stalling compared to RAM.

Memory in cache is checked using hashmap for O(1) access time , when cache gets full, First data that came to cache is removed (FIFO Eviction)


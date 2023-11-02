Fewest cycles overall:
-	Configuration: sets=256, blocks=4, eviction=lru, write_alloc=write-allocate, write_policy=write-back, trace=swim.trace
-	Total loads: 220668
-	Total stores: 82525
-	Load hits: 219507
-	Load misses: 1161
-	Store hits: 71956
-	Store misses: 10569
-	Total cycles: 8483931
-	Load Hit Percentage: 99.00%
-	Store Hit Percentage: 87.00%
-	Total Cycles: 8483931


Fewest cycles with direct:
-	Configuration: sets=256, blocks=1, eviction=fifo, write_alloc=no-write-allocate, write_policy=write-through, trace=swim.trace
-	Total loads: 220668
-	Total stores: 82525
-	Load hits: 213913
-	Load misses: 6755
-	Store hits: 56637
-	Store misses: 25888
-	Total cycles: 11231805
-	Load Hit Percentage: 96.00%
-	Store Hit Percentage: 68.00%
-	Total Cycles: 11231805

Fewest cycles with associative:
-	Configuration: sets=1, blocks=4, eviction=fifo, write_alloc=no-write-allocate, write_policy=write-through, trace=swim.trace
-	Total loads: 220668
-	Total stores: 82525
-	Load hits: 59277
-	Load misses: 161391
-	Store hits: 22390
-	Store misses: 60135
-	Total cycles: 73051958
-	Load Hit Percentage: 26.00%
-	Store Hit Percentage: 27.00%
-	Total Cycles: 73051958

Fewest cycles with lru:
-	See fewest cycles overall

Fewest cycles with fifo:
-	Configuration: sets=256, blocks=4, eviction=fifo, write_alloc=write-allocate, write_policy=write-back, trace=swim.trace
-	Total loads: 220668
-	Total stores: 82525
-	Load hits: 218357
-	Load misses: 2311
-	Store hits: 71787
-	Store misses: 10738
-	Total cycles: 9129502
-	Load Hit Percentage: 98.00%
-	Store Hit Percentage: 86.00%
-	Total Cycles: 9129502

Fewest cycles with write allocate and write back:
-	See fewest cycles overall

Fewest cycles with no write allocate and write through:
-	Configuration: sets=256, blocks=4, eviction=lru, write_alloc=no-write-allocate, write_policy=write-through, trace=swim.trace
-	Total loads: 220668
-	Total stores: 82525
-	Load hits: 218072
-	Load misses: 2596
-	Store hits: 58030
-	Store misses: 24495
-	Total cycles: 9569598
-	Load Hit Percentage: 98.00%
-	Store Hit Percentage: 70.00%
-	Total Cycles: 9569598

Fewest cycles with write allocate and write through:
-	Configuration: sets=256, blocks=4, eviction=lru, write_alloc=write-allocate, write_policy=write-through, trace=swim.trace
-	Total loads: 220668
-	Total stores: 82525
-	Load hits: 219507
-	Load misses: 1161
-	Store hits: 71956
-	Store misses: 10569
-	Total cycles: 12190793
-	Load Hit Percentage: 99.00%
-	Store Hit Percentage: 87.00%
-	Total Cycles: 12190793

    We ran a script that tested every combination of sets, blocks, eviction policies, and write hit and miss policies. 
We chose to run this script on swim.trace and gcc.trace because they had the biggest sample sizes which allowed us to have 
the most accurate hit percentage results. The following results are based on the swim.trace file.

    The fewest cycles we had was 8,483,931, which came from the following configuration: sets=256, blocks=4, eviction=lru, 
write_alloc=write-allocate, write_policy=write-back. This configuration is a 256-set 4-way set associative cache. 
This configuration also had the greatest load and store hit percentages. The fewest cycles with direct mapping was 11,231,805.
The fewest cycles with fully associative mapping was 73,051,958. This configuration also had the lowest load and store hit 
percentages. 

    LRU eviction policy had fewer cycles than FIFO, since our lowest overall cycles came from a configuration using LRU. 
The fewest cycles with FIFO was 9,129,502. 

    The configuration with the fewest overall cycles had a write allocate and write back write policy, meaning the write 
allocate and write back combination had the fewest cycles. The fewest cycles with no write allocate and write through policy 
was 9,569,598. The fewest cycles with write allocate and write through policy was 12,190,793. 

    In conclusion, the best cache configuration is a 256-set 4-way set associative cache with LRU eviction policy and write 
allocate and write back write policy because it uses the fewest cycles and gets the highest percentage of load and store hits. 

Contributions:

MS1: 
    Jayden: Makefile, main
    Nicolas: Main error checking

MS2:
    Jayden:
        - set up cache.h, set.h, slot.h, and cache.cpp
        - wrote setters and getters for cache, set, and slot
        - wrote constructor for cache.cpp
        - implemented storeAddress
        - helped debug loadAddress
        - implemented getTag, getIndex, getOffset
        - wrote file reading in main
    Nicolas:
        - transferred function implementations from cache.h into cache.cpp
        - debugged cache.cpp constructor
        - debugged getTag, getIndex, getOffset
        - implemented loadAddress
        - helped debug storeAddress
        - wrote constructor for set.cpp
        - wrote printStatistics
        - wrote file reading and error handling in main

MS3:
    Jayden:
        - implemented fifo
        - debugged loadAddress and storeAddress to get closer to the correct cycles
        - analyzed stats and wrote report
    Nicolas:
        - did final debugging for loadAddress and storeAddress to get full points on autograder
        - wrote script to output stats with every configuration combination in sorted cycle order



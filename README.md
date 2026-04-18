# Index based linked list

Investigation into whether a linked list where the next/prev 'pointers' are instead indices into an array can be faster than EASTL's intrusive linked list implementation in niche scenarios.

The idea is that if the next/prev pointers are indices, we can save memory overhead: it saves 8 bytes/node for 32 bit indices, 12 bytes for 16 bit indices and if you're really crazy 14 bytes for 8 bit indices. So for data sets sizes which are close to the boundary between the different caches or main memory, this may see a performance benefit by allowing the data to be fully in cache.

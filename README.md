# MemoryAllocators

Usage is very easy, see `example.cpp`.

## TODO:
- [ ] Stack Allocator
- [ ] Arena Allocator
- [ ] Relocatable Heap
- [ ] Move to C
- [x] PoolAllocator::Alloc should return a `void*` and it should be used to lookup the pool memory.

## Learned
- [0xCDCDCD... is returned in debug mode when allocating and something went bad.](https://en.wikipedia.org/wiki/Magic_number_(programming)#cite_ref-Win32CRTDebugHeapInternals_23-2)
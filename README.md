# Custom Memory Allocator (CSE 29 – Programming Assignment V)

This project implements a simplified version of `malloc` and `free` by managing a simulated heap.  
The allocator provides dynamic memory allocation and deallocation functions (`vmalloc` and `vmfree`) as well as utilities for heap initialization, inspection, and persistence.

## Features
- **Heap simulation:** Initializes a virtual heap using `mmap` from `/dev/zero`.
- **Custom allocator:** Implements `vmalloc(size_t)` to find best-fit free blocks, split them if needed, and return aligned pointers.
- **Deallocation with coalescing:** `vmfree(void *)` merges adjacent free blocks to reduce fragmentation.
- **Heap inspection:** `vminfo()` prints a formatted view of the heap, including offsets, sizes, free/busy status, and largest free block:contentReference[oaicite:0]{index=0}.
- **Heap persistence:** Save a heap to disk (`vmdump`) and reload it (`vmload`):contentReference[oaicite:1]{index=1}.
- **Alignment guarantees:** All allocated blocks are aligned to 16-byte boundaries:contentReference[oaicite:2]{index=2}.
- **Testing utility:** `vmtest.c` demonstrates heap initialization, allocation, and visualization:contentReference[oaicite:3]{index=3}.

## File Overview
- **`vmlib.h`** – Public API for the allocator   
- **`vm.h`** – Internal data structures and constants (
- **`vminit.c`** – Heap setup/teardown using `mmap`; defines `heapstart`
- **`vmalloc.c`** – Implementation of `vmalloc` 
- **`vmfree.c`** – Implementation of `vmfree`  
- **`utils.c`** – Utility functions (helpers for heap management)  
- **`Makefile`** – Build rules for compiling the allocator and tests  

#include "vm.h"
#include "vmlib.h"

/**
 * The vmfree() function frees the memory space pointed to by ptr,
 * which must have been returned by a previous call to vmmalloc().
 * Otherwise, or if free(ptr) has already been called before,
 * undefined behavior occurs.
 * If ptr is NULL, no operation is performed.
 */
void vmfree(void *ptr){
	
	if (ptr == NULL) {
        	return;
   	 }
	struct block_header *curr_block = (struct block_header *)((char *)ptr - sizeof(struct block_header));
	if ((curr_block->size_status & VM_BUSY) == 0) {
        	return;
	}
	//Trick for clearing mask.
	curr_block->size_status &= ~VM_BUSY;
	size_t curr_size = curr_block->size_status & VM_BLKSZMASK;

	if ((curr_block->size_status & VM_PREVBUSY) == 0) {
        	struct block_footer *prev_footer = (struct block_footer *)((char *)curr_block - sizeof(struct block_footer));
        	size_t prev_size = prev_footer->size;
        	struct block_header *prev_block = (struct block_header *)((char *)curr_block - prev_size);

        	curr_size += prev_size;
        	curr_block = prev_block;
        	curr_block->size_status = (curr_size | (curr_block->size_status & VM_PREVBUSY));
	}
	struct block_header *next_block = (struct block_header *)((char *)curr_block + curr_size);

	if (next_block->size_status != VM_ENDMARK) {
        	if ((next_block->size_status & VM_BUSY) == 0) {
            		size_t next_size = next_block->size_status & VM_BLKSZMASK;
            		curr_size += next_size;

            		curr_block->size_status = (curr_size | (curr_block->size_status & VM_PREVBUSY));

            		struct block_footer *footer = (struct block_footer *)((char *)curr_block + curr_size - sizeof(struct block_footer));
            		footer->size = curr_size;

            		struct block_header *after_next = (struct block_header *)((char *)next_block + next_size);
           		 if (after_next->size_status != VM_ENDMARK) {
                		after_next->size_status &= ~VM_PREVBUSY;
            		}
        	} 
		else {
            		next_block->size_status &= ~VM_PREVBUSY;

          		struct block_footer *footer = (struct block_footer *)((char *)curr_block + curr_size - sizeof(struct block_footer));
            		footer->size = curr_size;
        	}
	} 
    	else {
        	struct block_footer *footer = (struct block_footer *)((char *)curr_block + curr_size - sizeof(struct block_footer));
        	footer->size = curr_size;
    	}
}
	
	
	
	
		
	



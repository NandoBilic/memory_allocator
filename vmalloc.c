#include "vm.h"
#include "vmlib.h"

void *vmalloc(size_t size){
	if (size <= 0 ) {
		return NULL;
	}

	struct block_header *curr_header = heapstart;
	struct block_header *best_block = NULL;
	size_t size_requested = size + sizeof(struct block_header);
	//Set first blocks prev to busy.
	curr_header->size_status = curr_header->size_status | VM_PREVBUSY;
	
	if( size_requested % BLKALIGN != 0) {
		size_requested += BLKALIGN - (size_requested % BLKALIGN);
	}

	while(curr_header->size_status != VM_ENDMARK) {
		size_t header  = curr_header->size_status;
		// 1 = taken 0 = open
		uint8_t allocated = header & 0x1;
		uint8_t prev = (header >>1) & 0x1;
		size_t curr_size = header & VM_BLKSZMASK;

		/*if (prev == 1 ) {
			struct block_footer *foot = (struct block_footer*) (curr_header - 1);
			size_t footer_size = foot->size;

			curr_header->size_status = (footer_size + curr_size);			
			curr_header = (struct block_header*) ((char*)curr_header - footer_size - 8);
			continue;
		}*/
		      	
		

		if ( size_requested <= curr_size && allocated == 0) {
			size_t best_fit = curr_size - size_requested;
			if (best_block == NULL) {
				best_block = curr_header;
			}
			else{
				size_t curr_best = (best_block->size_status & VM_BLKSZMASK) - size_requested;
				if (best_fit < curr_best) {
					best_block = curr_header;
				}
			}
		}
		curr_header = (struct block_header *) ((char *)curr_header + curr_size);
	}

	if (best_block != NULL) {

		// Never split the block. Need to preserve the next block

		uint8_t prev = (best_block->size_status >> 1) & 0x1;
		size_t block_size = best_block->size_status & VM_BLKSZMASK;
		//Added sizeof seems like next was pointing in the allocated space. DID NOT WORK but I think in right direction.
		if (block_size - size_requested >= (sizeof(struct block_header) + BLKALIGN)) {
			struct block_header *next = (struct block_header*) ((char*)best_block + size_requested);
			next->size_status = block_size - size_requested;
			struct block_footer *free_foot =(struct block_footer*) ((char*) next + next->size_status - sizeof(struct block_footer));
			free_foot->size = next->size_status;
			next->size_status = next->size_status | VM_PREVBUSY;
			best_block->size_status = size_requested | VM_BUSY;
			if (prev == 1) {
				best_block->size_status = best_block->size_status | VM_PREVBUSY;
			}
		}
		else{
			struct block_header *next = (struct block_header*) ((char*)best_block + block_size);
			best_block->size_status = block_size | VM_BUSY;
			if (prev ==1) {
				best_block->size_status = best_block->size_status | VM_PREVBUSY;
			}
			if (next->size_status != VM_ENDMARK) {
				next->size_status = next->size_status | VM_PREVBUSY;
			}
		}

		return (void*) ((char*)best_block + sizeof(struct block_header));
	}	
	
	return NULL;
}

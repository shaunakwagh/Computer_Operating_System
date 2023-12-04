/*
 * kernel_code.c - Project 2, CMPSC 473
 * Copyright 2021 Ruslan Nikolaev <rnikola@psu.edu>
 * Distribution, modification, or usage without explicit author's permission
 * is not allowed.
 */

#include <kernel.h>
#include <types.h>
#include <printf.h>
#include <malloc.h>
#include <string.h>


void *page_table = NULL; /* TODO: Must be initialized to the page table address */
void *user_stack = NULL; /* TODO: Must be initialized to a user stack virtual address */
void *user_program = NULL; /* TODO: Must be initialized to a user program virtual address */


typedef unsigned long long u64;

	struct page_pte{
	u64 present:1;       // Bit P
    	u64 writable:1;      // Bit R/W
    	u64 user_mode:1;     // Bit U/S
    	u64 unused:9;        // Unused bits 

    
    	u64 page_address:40; // 40+12 = 52-bit physical address (max)
    	u64 avail:7;         // reserved, should be 0
    	u64 pke:4;           // no MPK/PKE, should be 0
    	u64 nonexecute:1;

	};
	


// structure for PDE

struct page_pde{
	u64 present:1;       // Bit P
    	u64 writable:1;      // Bit R/W
    	u64 user_mode:1;     // Bit U/S
    	u64 unused:9;        // Unused bits 

    
    	u64 page_address:40; // 40+12 = 52-bit physical address (max)
    	u64 avail:7;         // reserved, should be 0
    	u64 pke:4;           // no MPK/PKE, should be 0
    	u64 nonexecute:1;

	};
	
	
	


// structure for PDPE

struct page_pdpe{
	u64 present:1;       // Bit P
    	u64 writable:1;      // Bit R/W
    	u64 user_mode:1;     // Bit U/S
    	u64 unused:9;        // Unused bits 

    
    	u64 page_address:40; // 40+12 = 52-bit physical address (max)
    	u64 avail:7;         // reserved, should be 0
    	u64 pke:4;           // no MPK/PKE, should be 0
    	u64 nonexecute:1;

	};



// structure for PML4E


struct page_pml4e{
    u64 present:1;       // Bit P
    u64 writable:1;      // Bit R/W
    u64 user_mode:1;     // Bit U/S
    u64 unused:9;        // Unused bits 

    
    u64 page_address:40; // 40+12 = 52-bit physical address (max)
    u64 avail:7;         // reserved, should be 0
    u64 pke:4;           // no MPK/PKE, should be 0
    u64 nonexecute:1;

	};






void kernel_init(void *ustack, void *uprogram, void *memory, size_t memorySize)
{

	printf("Hello from sgroup 54, ashwathraghav ,ssw5250\n\n");
	// 'memory' points to the place where memory can be used to create
	// page tables (assume 1:1 initial virtual-to-physical mappings here)
	// 'memorySize' is the maximum allowed size, do not exceed that (given just in case)
	
	// TODO: Create a page table here
	

	struct page_pte *p = (struct page_pte *)memory; // Each entry is 8 bytes
	for (int i = 0; i < 1048576; i++) { // 1048576/512 = 2048 pages
	// physical pages 0,1,2,3,...,1048575 (absolute address)
		//printf("in pte for loop\n");
	p[i].page_address = i;
	p[i].writable = 1;
	p[i].present = 1;
	p[i].user_mode = 0;
	p[i].unused = 0;
	p[i].avail = 0;
	p[i].pke = 0;
	p[i].nonexecute = 0;
	}
	
	struct page_pde *pde = (struct page_pde *) (p + 1048576); 
	for (int j = 0; j < 2048; j++) { // 2048/512 = 4 pages
	struct page_pte *start_pte = (struct page_pte *) p + 512 * j;
	pde[j].page_address = (u64) start_pte >> 12; // PTE page address
	pde[j].writable = 1;
	pde[j].present = 1;
	pde[j].user_mode = 0;
	pde[j].unused = 0;
	pde[j].avail = 0;
	pde[j].pke = 0;
	pde[j].nonexecute = 0;
	}

	
	


	struct page_pdpe *pdpe = (struct page_pdpe *)(pde + 2048);
	for (int k = 0; k < 4; k++)
	{
		struct page_pde *start_pde = (struct page_pde *) pde + 512 * k;
		pdpe[k].page_address = (u64) start_pde >> 12;
		pdpe[k].writable = 1;
		pdpe[k].present = 1;
		pdpe[k].user_mode = 0;
		pdpe[k].unused = 0;
		pdpe[k].avail = 0;
		pdpe[k].pke = 0;
		pdpe[k].nonexecute = 0;
	}
	
	
	for(int l = 4; l < 512; l++)
	{
		pdpe[l].page_address=0;
		pdpe[l].writable = 0;
		pdpe[l].present = 0;
		pdpe[l].user_mode = 0;
		pdpe[l].unused = 0;
		pdpe[l].avail = 0;
		pdpe[l].pke = 0;
		pdpe[l].nonexecute = 0;
	}
	
	

	struct page_pml4e *pml4e = (struct page_pml4e *)(pdpe + 512);
	pml4e[0].page_address = (u64) pdpe >> 12;
	pml4e[0].writable = 1;
	pml4e[0].present = 1;
	pml4e[0].user_mode = 0;
	pml4e[0].unused = 0;
	pml4e[0].avail = 0;
	pml4e[0].pke = 0;
	pml4e[0].nonexecute = 0;

	for(int m = 1; m < 511; m++)
	{
		pml4e[m].page_address = 0;
		pml4e[m].writable = 0;
		pml4e[m].present = 0;
		pml4e[m].user_mode = 0;
		pml4e[m].unused = 0;
		pml4e[m].avail = 0;
		pml4e[m].pke = 0;
		pml4e[m].nonexecute = 0;
	}
	//printf("pml4e[0]=%llx\n", (uint64_t) pml4e[0].page_address);
	//printf("pdpe[0]=%llx\n", (uint64_t) pdpe[0].page_address);
	//printf("pde[0]=%llx\n", (uint64_t) pde[0].page_address);
	//printf("pte[0]=%llx\n", (uint64_t) p[0].page_address);
	

	// TODO: It is OK for Q1-Q3, but should changed
	// for the final part's Q4 when creating your own user page table
	user_stack = ustack;

	// TODO: It is OK for Q1-Q3, but should changed
	// for the final part's Q4 when creating your own user page table
	user_program = uprogram;
	
	/*user_stack = (void *)(0xFFFFFFFFFFFFFFFF-4096);
	user_program = user_stack;
	
	struct page_pte *user_pte = (struct page_pte *)pdpe + 512;
	for (int i = 0; i < 510; i++) {
	struct page_pdpe *start_pdpe = (struct page_pdpe *) pdpe + i; 
	user_pte[i].page_address = (u64) start_pdpe >> 12;
	user_pte[i].writable = 1;
	user_pte[i].present = 1;
	user_pte[i].user_mode = 1;
	user_pte[i].unused = 0;
	user_pte[i].avail = 0;
	user_pte[i].pke = 0;
	user_pte[i].nonexecute = 0;
	}
	user_pte[510].page_address = (u64)(ustack-4096) >> 12;
	user_pte[510].writable = 1;
	user_pte[510].present = 1;
	user_pte[510].user_mode = 1;
	user_pte[510].unused = 0;
	user_pte[510].avail = 0;
	user_pte[510].pke = 0;
	user_pte[510].nonexecute = 0;
	
	user_pte[511].page_address = (u64) uprogram >> 12;
	user_pte[511].writable = 1;
	user_pte[511].present = 1;
	user_pte[511].user_mode = 1;
	user_pte[511].unused = 0;
	user_pte[511].avail = 0;
	user_pte[511].pke = 0;
	user_pte[511].nonexecute = 0;
	
	
	
	struct page_pde *user_pde = (struct page_pde *) user_pte + 512 ;
	for (int j = 0; j < 511; j++) { 
	struct page_pte *user_start_pte = (struct page_pte *) user_pte + j;
	user_pde[j].page_address = (u64) user_start_pte >> 12;
	user_pde[j].writable = 1;
	user_pde[j].present = 1;
	user_pde[j].user_mode = 1;
	user_pde[j].unused = 0;
	user_pde[j].avail = 0;
	user_pde[j].pke = 0;
	user_pde[j].nonexecute = 0;
	}
	user_pde[511].page_address = (u64) user_pte >> 12;
	user_pde[511].writable = 1;
	user_pde[511].present = 1;
	user_pde[511].user_mode = 1;
	user_pde[511].unused = 0;
	user_pde[511].avail = 0;
	user_pde[511].pke = 0;
	user_pde[511].nonexecute = 0;







	struct page_pdpe *user_pdpe = (struct page_pdpe *) user_pde + 512;
	for (int k = 0; k < 511; k++) {
	struct page_pde *user_start_pde = (struct page_pde *) user_pde + k;
	user_pdpe[k].page_address = (u64) user_start_pde >> 12; 
	user_pdpe[k].writable = 1;
	user_pdpe[k].present = 1;
	user_pdpe[k].user_mode = 1;
	user_pdpe[k].unused = 0;
	user_pdpe[k].avail = 0;
	user_pdpe[k].pke = 0;
	user_pdpe[k].nonexecute = 0;
	}
	
	user_pdpe[511].page_address = (u64) user_pde >> 12; 
	user_pdpe[511].writable = 1;
	user_pdpe[511].present = 1;
	user_pdpe[511].user_mode = 1;
	user_pdpe[511].unused = 0;
	user_pdpe[511].avail = 0;
	user_pdpe[511].pke = 0;
	user_pdpe[511].nonexecute = 0;


	
	struct page_pml4e *pml4e = (struct page_pml4e *)(user_pdpe + 512);
	pml4e[0].page_address = (u64) pdpe >> 12;
	pml4e[0].writable = 1;
	pml4e[0].present = 1;
	pml4e[0].user_mode = 0;
	pml4e[0].unused = 0;
	pml4e[0].avail = 0;
	pml4e[0].pke = 0;
	pml4e[0].nonexecute = 0;

	for(int m = 1; m < 511; m++)
	{
		pml4e[m].page_address = 0;
		pml4e[m].writable = 0;
		pml4e[m].present = 0;
		pml4e[m].user_mode = 0;
		pml4e[m].unused = 0;
		pml4e[m].avail = 0;
		pml4e[m].pke = 0;
		pml4e[m].nonexecute = 0;
	}
	pml4e[511].page_address = (u64) (user_pdpe ) >> 12;
	pml4e[511].writable = 0;
	pml4e[511].present = 0;
	pml4e[511].user_mode = 0;
	pml4e[511].unused = 0;
	pml4e[511].avail = 0;
	pml4e[511].pke = 0;
	pml4e[511].nonexecute = 0;	
	// The remaining portion just loads the page table,
	// this does not need to be changed:
	// load 'page_table' into the CR3 register
	*/
	page_table = (void *) pml4e;
	const char *err = load_page_table(page_table);
	if (err != NULL) {
		printf("ERROR: %s\n", err);
	}
	
	// The extra credit assignment
	mem_extra_test();
}

/* The entry point for all system calls */
long syscall_entry(long n, long a1, long a2, long a3, long a4, long a5)
{
	// TODO: the system call handler to print a message (n = 1)
	// the system call number is in 'n', make sure it is valid!
	if((int)n==1)
	{
		printf("%s\n", (char *)a1);
		return 0;
	}
		/*printf("\nSystem call number is not valid!\n");
		printf("%ld",n);
		return -1;*/
	// Arguments are passed in a1,.., a5 and can be of any type
	// (including pointers, which are casted to 'long')
	// For the project, we only use a1 which will contain the address
	// of a string, cast it to a pointer appropriately 

	// For simplicity, assume that the address supplied by the
	// user program is correct
	//
	// Hint: see how 'printf' is used above, you want something very
	// similar here
	return -1; /* Success: 0, Failure: -1 */
}

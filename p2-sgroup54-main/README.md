# Final Submission Work

We were asked to implement a simple system call handler, user space, user level and kernel level page tables.

We handled the system call in sys_call entry function. The specific system call to be handled has system call number = 1(n = 1). THe purpose of this system call is to print a string from user space. The address of the spring is passed to the syscall function as a1. We returned an error status (-1) in all other cases. 

We were asked to implement a 4 level page table for Q3. We implemented kernel level page tables pte, pde, pdpe and pml4e and each of the entries were of the size 8 bytes. We had 1 page in pml4e, 4 pages in pdpe, 2048 pages in pde and 1048576 pages in pte.

We implemented user level page tables as well for q4 with proper logic but the code crashed. We have implemented the extra credits portion as well where we were asked to implement malloc, init and free functions.

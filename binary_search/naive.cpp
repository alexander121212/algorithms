#include "naive.h"

/*
 * │3   Disassembly of section .text: 
 * │
 * │5   0000000000001450 <naive_lower_bound(int, int const*, int)>:  
 * │6   _Z17naive_lower_boundiPKii():                                
 * │      endbr64                                                    
 * │      dec     %edx                                               
 * │      xor     %ecx,%ecx                                          
 * │    ↓ jmp     20                                                 
 * │      nop                                                        
 * │10:   lea     (%rdx,%rcx,1),%eax                                 
 * │      sar     %eax                                               
 * │      movslq  %eax,%r8                                           
 * │      cmp     %edi,(%rsi,%r8,4)                                  
 * │    ↓ jl      30                                                 
 * │      mov     %eax,%edx                                          
 * │20:   cmp     %ecx,%edx                                          
 * │    ↑ jg      10                                                 
 * │      movslq  %ecx,%rdx                                          
 * │      mov     (%rsi,%rdx,4),%eax                                 
 * │    ← retq                                                       
 * │      nop                                                        
 * │30:   lea     0x1(%rax),%ecx                                     
 * │    ↑ jmp     20
 */

int naive_lower_bound(int x, const int *arr, int size)
{
	int left = 0, right = size-1;

	while (left < right) {
		int middle = (left + right) / 2;
		if (arr[middle] >= x)
			right = middle;
		else
			left = middle + 1;
	}

	return arr[left];
}


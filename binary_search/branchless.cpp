#include "branchless.h"

/*
 * │    0000000000001450 <branchless_lower_bound(int, int const*, int)>:  
 * │    _Z22branchless_lower_boundiPKii():                                
 * │      endbr64                                                         
 * │      cmp        $0x1,%edx                                            
 * │    ↓ jle        49                                                   
 * │      xor        %r9d,%r9d                                            
 * │      nop                                                             
 * │10:┌─→mov        %edx,%ecx                                            
 * │   │  sar        %ecx                                                 
 * │   │  sub        %ecx,%edx                                            
 * │   │  mov        %edx,%eax                                            
 * │   │  shr        $0x1f,%eax                                           
 * │   │  add        %edx,%eax                                            
 * │   │  sar        %eax                                                 
 * │   │  movslq     %ecx,%r10                                            
 * │   │  movslq     %eax,%r8                                             
 * │   │  add        %ecx,%eax                                            
 * │   │  cmp        %edi,-0x4(%rsi,%r10,4)                               
 * │   │  cmovge     %r9d,%ecx                                            
 * │   │  cltq                                                            
 * │   │  movslq     %ecx,%r11                                            
 * │   │  prefetcht0 -0x4(%rsi,%r8,4)                                     
 * │   │  prefetcht0 -0x4(%rsi,%rax,4)                                    
 * │   │  lea        (%rsi,%r11,4),%rsi                                   
 * │   ├──cmp        $0x1,%edx                                            
 * │   └──jg         10                                                   
 * │49:   mov        (%rsi),%eax                                          
 * │    ← retq
 */
int branchless_lower_bound(int x, const int *arr, int size)
{
	const int *base = arr;
	int len = size;

	while (len > 1) {
		int half = len / 2;
		len -= half;

		__builtin_prefetch((void*)&base[len/2 - 1]);
		__builtin_prefetch((void*)&base[half + len/2 - 1]);

		base += (base[half - 1] < x) * half;
	}

	return *base;
}

/*
 *        │    0000000000001490 <eytzynger(int, int const*, int*, int)>:
 *      │    _ZL9eytzyngeriPKiPii():                                    
 * 0,95 │      cmp    %ecx,%edi                                         
 * 2,84 │    ↓ jg     60                                                
 * 3,79 │      push   %r14                                              
 *      │      mov    %rsi,%r14                                         
 * 2,85 │      push   %r13                                              
 * 0,94 │      mov    %rdx,%r13                                         
 * 2,85 │      push   %r12                                              
 *      │      push   %rbp                                              
 *      │      mov    %ecx,%ebp                                         
 * 2,82 │      push   %rbx                                              
 * 3,77 │      mov    %edi,%ebx                                         
 * 7,58 │16:   lea    (%rbx,%rbx,1),%r12d                               
 * 0,92 │      mov    %ebp,%ecx                                         
 * 2,82 │      mov    %r13,%rdx                                         
 * 2,81 │      mov    %r14,%rsi                                         
 * 2,85 │      mov    %r12d,%edi                                        
 * 4,69 │    → callq  eytzynger                                         
 *      │      movslq eytzynger(int, int const*, int*, int)::i,%rax     
 * 3,78 │      movslq %ebx,%rbx                                         
 *12,25 │      mov    (%r14,%rax,4),%ecx                                
 * 7,56 │      lea    0x1(%rax),%edx                                    
 *10,35 │      mov    %ecx,0x0(%r13,%rbx,4)                             
 * 2,81 │      lea    0x1(%r12),%ebx                                    
 *      │      mov    %edx,eytzynger(int, int const*, int*, int)::i     
 * 7,57 │      cmp    %ebp,%ebx                                         
 *      │    ↑ jle    16                                                
 * 0,88 │      pop    %rbx                                              
 *      │      pop    %rbp                                              
 *      │      pop    %r12                                              
 * 3,79 │      pop    %r13                                              
 * 0,94 │      pop    %r14                                              
 *      │    ← retq                                                     
 *      │      nop                                                      
 * 7,57 │60: ← retq
 */
void eytzynger(int k, const int *arr, int *ey_arr, int size)
{
	static int i = 0;

	if (k <= size) {
		eytzynger(2*k, arr, ey_arr, size);
		ey_arr[k] = arr[i++];
		eytzynger(2*k+1, arr, ey_arr, size);
	}
}


static inline int __lg(int x)
{
    return 32 - __builtin_clz(x) - 1;
}


int eytzinger_layout_lower_bound(int x, int *ey_arr, const int *arr, int size)
{
	int k = 1;
	int iters = __lg(size + 1);

	for (int i = 0; i < iters; i++) {
		k = 2 *k + (ey_arr[k] < x);
	}

	int *loc =  (k <= size ? ey_arr + k : ey_arr);

	k = 2 * k + (*loc < x);
	k >>= __builtin_ffs(~k);

	return ey_arr[k];
}

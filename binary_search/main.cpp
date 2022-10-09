#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <chrono>

#include "naive.h"
#include "branchless.h"
#include "eytzynger.h"

/* See ``dmidecode -t cache`` or ``lshw -class memory`` output */
#define L1CACHE (384*1024)
#define L2CACHE (1536*1024) 
#define L3CACHE (12288*1024)

typedef int (*lower_bound_t)(int x, const int *arr, int size);

int main(void)
{
	std::pair<lower_bound_t, std::string> tests[] = {
		{ naive_lower_bound, "naive" }, 
		{ branchless_lower_bound, "branchless" },
	};
	int *arr = new int[L3CACHE];
	int *ey_arr = (int*)std::aligned_alloc(64, 4 * (L3CACHE+1));

	for (int i = 0; i < L3CACHE; i++)
		arr[i] = i+1;

	eytzynger(1, arr, ey_arr, L3CACHE);
	ey_arr[0] = -1;
	for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
		auto start = std::chrono::high_resolution_clock::now();

		int res = tests[i].first(10000, arr, L3CACHE);

		auto stop = std::chrono::high_resolution_clock::now();

		std::cout << "Time taken by " << tests[i].second << " func: "
			<< std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()
			<< " microseconds. Result = " << res << std::endl;
	}

	eytzinger_layout_lower_bound(10000, arr, ey_arr, L3CACHE);

	std::free(ey_arr);
	delete[] arr;

	return 0;
}

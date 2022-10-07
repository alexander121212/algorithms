#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <algorithm>
#include <functional>
#include <array>
#include <iostream>
#include <chrono>

#include <x86intrin.h>
#include <bits/stdc++.h>

#include "naive.h"
#include "transposition.h"
#include "vectorization.h"

using namespace std;
using namespace std::chrono;


void test_sse_avx_support(void)
{
	cout << __builtin_cpu_supports("sse") << endl;
	cout << __builtin_cpu_supports("sse2") << endl;

	cout << __builtin_cpu_supports("avx") << endl;
	cout << __builtin_cpu_supports("avx2") << endl;
	cout << __builtin_cpu_supports("avx512f") << endl;
}

#define MATRIX_SIZE 1920

typedef void (*matr_mult_t)(const float *a, const float *b, float *c, int size);

int main(void)
{
	float *a = new float[MATRIX_SIZE*MATRIX_SIZE];
	float *b = new float[MATRIX_SIZE*MATRIX_SIZE];
	float *c = new float[MATRIX_SIZE*MATRIX_SIZE];

	std::pair<matr_mult_t, std::string> tests[] = {
		{ naive_matrix_multiptication, "naive" }, 
		{ transposition_matrix_mul, 	"transposition" }, 
		{ vectorization_matrix_mul, 	"vectorization" } 
	};

	test_sse_avx_support();

	for (int i = 0; i < MATRIX_SIZE*MATRIX_SIZE; i++) {
		a[i] = float(rand()) / RAND_MAX;
		b[i] = float(rand()) / RAND_MAX;
		c[i] = 0.0;
	}

	for (int i = 0; i < sizeof(tests)/sizeof(tests[0]); i++) {
		auto start = high_resolution_clock::now();
		tests[i].first(a, b, c, MATRIX_SIZE);
		auto stop = high_resolution_clock::now();

		cout << "Time taken by " << tests[i].second << " func: "
			<< duration_cast<microseconds>(stop - start).count()
			<< " microseconds" << endl;
	}


	delete[] a;
	delete[] b;
	delete[] c;

	return 0;
}


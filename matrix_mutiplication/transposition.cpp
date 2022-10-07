#include "transposition.h"


void transposition_matrix_mul(const float *a, const float *_b, float * __restrict__ c, int size)
{
	float *b = new float[size * size];

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			b[i*size + j] = _b[j*size + i];
		}
	}


	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			for (int k = 0; k < size; k++) {
				c[i*size + j] += a[i*size + k] * b[j*size + k];
			}
		}
	}

	delete[] b;
}

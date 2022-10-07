#include "naive.h"

void naive_matrix_multiptication(const float *a, const float *b, float *c, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			for (int k = 0; k < size; k++)
				c[i*size + j] += a[j*size + k] * b[k*size + j];
}

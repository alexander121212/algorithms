#include "vectorization.h"


typedef float vec __attribute__ (( vector_size(32)));

void vectorization_matrix_mul(const float *_a, const float *_b, float *c, int size)
{
	/* number of 8 element in matrix row */
	int nB = (size + 7) / 8;

	vec* a = (vec*)std::aligned_alloc(32, 32*size*nB);
	memset(a, 0, 32 * size);

	vec* b = (vec*)std::aligned_alloc(32, 32*size*nB);
	memset(b, 0, 32 * size);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			a[i*nB + j/8][j % 8] = _a[i*size + j];
			b[i*nB + j/8][j % 8] = _b[i*size + j];
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			vec s{};

			for (int k = 0; k < nB; k++) {
				s += a[i * nB + k] * b[j * nB + k];
			}

			for (int k = 0; k < 8; k++) {
				c[i*size + j] += s[k];
			}
		}
	}
	std::free(b);
	std::free(a);
}

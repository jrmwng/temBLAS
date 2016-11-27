#include "stdafx.h"
#include <intrin.h>

extern "C"
{
#include "scopy.h"
}

namespace temBLAS
{
	void scopy_avx(unsigned uN, float const *prX, int nStepX, float *prY, int nStepY)
	{
		if (nStepY == 1)
		{
			if (nStepX == 1)
			{
#ifdef __AVX__
				for (; 0x10 <= uN; uN -= 0x10, prX += 0x10 * nStepX, prY += 0x10 * nStepY)
				{
					__m256 r8A;
					__m256 r8B;
					{
						if (reinterpret_cast<uintptr_t>(prX) % 0x20)
						{
							r8A = _mm256_loadu_ps(prX + 0 * nStepX);
							r8B = _mm256_loadu_ps(prX + 8 * nStepX);
						}
						else
						{
							r8A = _mm256_load_ps(prX + 0 * nStepX);
							r8B = _mm256_load_ps(prX + 8 * nStepX);
						}
					}

					if (reinterpret_cast<uintptr_t>(prY) % 0x20)
					{
						_mm256_storeu_ps(prY + 0 * nStepY, r8A);
						_mm256_storeu_ps(prY + 8 * nStepY, r8B);
					}
					else
					{
						_mm256_store_ps(prY + 0 * nStepY, r8A);
						_mm256_store_ps(prY + 8 * nStepY, r8B);
					}
				}
#else
				for (; 0 <= uN; uN -= 8, prX += 8 * nStepX, prY += 8 * nStepY)
				{
					__m128 r4A;
					__m128 r4B;
					{
						if (reinterpret_cast<uintptr_t>(prX) % 0x10)
						{
							r4A = _mm_loadu_ps(prX + 0 * nStepX);
							r4B = _mm_loadu_ps(prX + 4 * nStepX);
						}
						else
						{
							r4A = _mm_load_ps(prX + 0 * nStepX);
							r4B = _mm_load_ps(prX + 4 * nStepX);
						}
					}

					if (reinterpret_cast<uintptr_t>(prY) % 0x10)
					{
						_mm_storeu_ps(prY + 0 * nStepY, r4A);
						_mm_storeu_ps(prY + 4 * nStepY, r4B);
					}
					else
					{
						_mm_store_ps(prY + 0 * nStepY, r4A);
						_mm_store_ps(prY + 4 * nStepY, r4B);
					}
				}
#endif
			}
#ifdef __AVX2__
			else if (nStepX)
			{
				__m256i const n8IndexX = _mm256_mullo_epi32(_mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0), _mm256_set1_epi32(nStepX));

				for (; 0x10 <= uN; uN -= 0x10, prX += 0x10 * nStepX, prY += 0x10 * nStepY)
				{
					__m256 const r8A = _mm256_i32gather_ps(prX + 0 * nStepX, n8IndexX, sizeof(float));
					__m256 const r8B = _mm256_i32gather_ps(prX + 8 * nStepX, n8IndexX, sizeof(float));

					if (reinterpret_cast<uintptr_t>(prY) % 0x20)
					{
						_mm256_storeu_ps(prY + 0 * nStepY, r8A);
						_mm256_storeu_ps(prY + 8 * nStepY, r8B);
					}
					else
					{
						_mm256_store_ps(prY + 0 * nStepY, r8A);
						_mm256_store_ps(prY + 8 * nStepY, r8B);
					}
				}
			}
#endif
		}
#ifdef __AVX512__
		else if (nStepY)
		{
			__m256i const n8IndexX = _mm256_mullo_epi32(_mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0), _mm256_set1_epi32(nStepX));
			__m256i const n8IndexY = _mm256_mullo_epi32(_mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0), _mm256_set1_epi32(nStepY));
			__m256i const n8IndexX8 = _mm256_slli_epi32(n8IndexX, 3);
			__m256i const n8IndexY8 = _mm256_slli_epi32(n8IndexY, 3);

			for (; 0x40 <= uN; uN -= 0x40, prX += 0x40 * nStepX, prY += 0x40 * nStepY)
			{
				__m256 const r8A = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 0), 0), n8IndexX8, sizeof(float));
				__m256 const r8B = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 0), 1), n8IndexX8, sizeof(float));
				__m256 const r8C = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 0), 2), n8IndexX8, sizeof(float));
				__m256 const r8D = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 0), 3), n8IndexX8, sizeof(float));
				__m256 const r8E = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 1), 0), n8IndexX8, sizeof(float));
				__m256 const r8F = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 1), 1), n8IndexX8, sizeof(float));
				__m256 const r8G = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 1), 2), n8IndexX8, sizeof(float));
				__m256 const r8H = _mm256_i32gather_ps(prX + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexX, 1), 3), n8IndexX8, sizeof(float));

				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 0), 0), n8IndexY, r8A, sizeof(float));
				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 0), 1), n8IndexY, r8B, sizeof(float));
				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 0), 2), n8IndexY, r8C, sizeof(float));
				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 0), 3), n8IndexY, r8D, sizeof(float));
				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 1), 0), n8IndexY, r8E, sizeof(float));
				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 1), 1), n8IndexY, r8F, sizeof(float));
				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 1), 2), n8IndexY, r8G, sizeof(float));
				_mm256_i32scatter_ps(prY + _mm_extract_epi32(_mm256_extracti128_si256(n8IndexY, 1), 3), n8IndexY, r8H, sizeof(float));
			}

			for (; 0x10 <= uN; uN -= 0x10, prX += 0x10 * nStepX, prY += 0x10 * nStepY)
			{
				__m256 const r8A = _mm256_i32gather_ps(prX + 0 * nStepX, n8IndexX, sizeof(float));
				__m256 const r8B = _mm256_i32gather_ps(prX + 8 * nStepX, n8IndexX, sizeof(float));

				_mm256_i32scatter_ps(prY + 0 * nStepY, n8IndexY, r8A, sizeof(float));
				_mm256_i32scatter_ps(PrY + 1 * nStepY, n8IndexY, r8B, sizeof(float));
			}
		}
#endif
		if (0 < uN)
		{
			SCOPY_ref(uN, prX, nStepX, prY, nStepY);
		}
	}
}

extern "C"
{
	void SCOPY_ref(unsigned uN, float const *prX, int nStepX, float *prY, int nStepY)
	{
		for (unsigned uIndex = 0; uIndex < uN; uIndex++, prX += nStepX, prY += nStepY)
		{
			*prY = *prX;
		}
	}
	void SCOPY_avx(unsigned uN, float const *prX, int nStepX, float *prY, int nStepY)
	{
		temBLAS::scopy_avx(uN, prX, nStepX, prY, nStepY);
	}
}

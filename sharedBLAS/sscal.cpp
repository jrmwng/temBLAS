#include "stdafx.h"
#include <intrin.h>

extern "C"
{
#include "sscal.h"
}

namespace temBLAS
{
	void sscal_avx(unsigned uN, float rAlpha, float *prX, int nStepX)
	{
#ifdef __AVX__
		__m256 const r8Alpha = _mm256_set1_ps(rAlpha);
#else
		__m128 const r4Alpha = _mm_set1_ps(rAlpha);
#endif

		if (nStepX == 1)
		{
#ifdef __AVX__
			for (; 16 <= uN; uN -= 16, prX += 16 * nStepX)
			{
				__m256 r8A;
				__m256 r8B;
				{
					if (reinterpret_cast<uintptr_t>(prX) % 32)
					{
						r8A = _mm256_loadu_ps(prX + 0);
						r8B = _mm256_loadu_ps(prX + 8);
					}
					else
					{
						r8A = _mm256_load_ps(prX + 0);
						r8B = _mm256_load_ps(prX + 8);
					}
				}

				__m256 const r8OutA = _mm256_mul_ps(r8A, r8Alpha);
				__m256 const r8OutB = _mm256_mul_ps(r8B, r8Alpha);

				if (reinterpret_cast<uintptr_t>(prX) % 32)
				{
					_mm256_storeu_ps(prX + 0, r8OutA);
					_mm256_storeu_ps(prX + 8, r8OutB);
				}
				else
				{
					_mm256_store_ps(prX + 0, r8OutA);
					_mm256_store_ps(prX + 8, r8OutB);
				}
			}
#else
			for (; 8 <= uN; uN -= 8, prX += 8 * nStepX)
			{
				__m128 r4A;
				__m128 r4B;
				{
					if (reinterpret_cast<uintptr_t>(prX) % 16)
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

				__m128 const r4OutA = _mm_mul_ps(r4A, r4Alpha);
				__m128 const r4OutB = _mm_mul_ps(r4B, r4Alpha);

				if (reinterpret_cast<uintptr_t>(prX) % 16)
				{
					_mm_storeu_ps(prX + 0 * nStepX, r4OutA);
					_mm_storeu_ps(prX + 4 * nStepX, r4OutB);
				}
				else
				{
					_mm_store_ps(prX + 0 * nStepX, r4OutA);
					_mm_store_ps(prX + 4 * nStepX, r4OutB);
				}
			}
#endif
		}
#ifdef __AVX2__
		else if (nStepX)
		{
			__m256i const n8Index = _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0);
			__m256i const n8IndexX = _mm256_mullo_epi32(n8Index, _mm256_set1_epi32(nStepX));
			__m256i const n8IndexX2 = _mm256_slli_epi32(n8IndexX, 1);

			for (; 16 <= uN; uN -= 16, prX += nStepX * 16)
			{
				__m256 const r8A = _mm256_i32gather_ps(prX + 0 * nStepX, n8IndexX2, sizeof(float));
				__m256 const r8B = _mm256_i32gather_ps(prX + 1 * nStepX, n8IndexX2, sizeof(float));

				__m256 const r8OutA = _mm256_mul_ps(r8A, r8Alpha);
				__m256 const r8OutB = _mm256_mul_ps(r8B, r8Alpha);

#ifdef __AVX512__
				_mm256_i32scatter_ps(prX + 0 * nStepX, n8IndexX2, r8OutA, sizeof(float));
				_mm256_i32scatter_ps(prX + 1 * nStepX, n8IndexX2, r8OutB, sizeof(float));
#else
				int *pnX = reinterpret_cast<int *>(prX);

				pnX[nStepX * 0x0] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 0), 0);
				pnX[nStepX * 0x1] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 0), 0);
				pnX[nStepX * 0x2] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 0), 1);
				pnX[nStepX * 0x3] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 0), 1);
				pnX[nStepX * 0x4] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 0), 2);
				pnX[nStepX * 0x5] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 0), 2);
				pnX[nStepX * 0x6] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 0), 3);
				pnX[nStepX * 0x7] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 0), 3);
				pnX[nStepX * 0x8] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 1), 0);
				pnX[nStepX * 0x9] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 1), 0);
				pnX[nStepX * 0xA] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 1), 1);
				pnX[nStepX * 0xB] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 1), 1);
				pnX[nStepX * 0xC] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 1), 2);
				pnX[nStepX * 0xD] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 1), 2);
				pnX[nStepX * 0xE] = _mm_extract_ps(_mm256_extractf128_ps(r8OutA, 1), 3);
				pnX[nStepX * 0xF] = _mm_extract_ps(_mm256_extractf128_ps(r8OutB, 1), 3);
#endif
			}
		}
#endif
		if (0 < uN)
		{
			SSCAL_ref(uN, rAlpha, prX, nStepX);
		}
	}
}

extern "C"
{
	void SSCAL_ref(unsigned uN, float rAlpha, float *prX, int nStepX)
	{
		for (unsigned uIndex = 0; uIndex < uN; uIndex++, prX += nStepX)
		{
			*prX *= rAlpha;
		}
	}
	void SSCAL_avx(unsigned uN, float rAlpha, float *prX, int nStepX)
	{
		temBLAS::sscal_avx(uN, rAlpha, prX, nStepX);
	}
}
#include "stdafx.h"
#include "CppUnitTest.h"

extern "C"
{
#include "..\sharedBLAS\sscal.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testBLAS
{
	TEST_CLASS(testBLAS)
	{
		template <unsigned uSIZE, unsigned uSTEPX, short wALPHA>
		std::enable_if_t<(uSIZE && uSTEPX)> test_sscal()
		{
			float arXref[uSIZE * uSTEPX];
			float arXavx[uSIZE * uSTEPX];
			{
				for (unsigned short uIndex = 0; uIndex < uSIZE * uSTEPX; uIndex++)
				{
					arXref[uIndex] = uIndex;
					arXavx[uIndex] = uIndex;
				}
				SSCAL_ref(uSIZE, wALPHA, arXref, uSTEPX);
				SSCAL_avx(uSIZE, wALPHA, arXavx, uSTEPX);
			}
			for (unsigned uIndexX = 0; uIndexX < uSIZE * uSTEPX; uIndexX += uSTEPX)
			{
				if (arXavx[uIndexX] != arXref[uIndexX])
				{
					Assert::AreEqual(arXref[uIndexX], arXavx[uIndexX]);
				}
			}
		}
		template <unsigned uSIZE, unsigned uSTEPX, short wALPHA>
		std::enable_if_t<!(uSIZE && uSTEPX)> test_sscal()
		{
		}
	public:
		TEST_METHOD(TestMethod_SSCAL)
		{
			for_each(std::make_integer_sequence<unsigned, 32>(), [&](auto const uSize)
			{
				for_each(std::make_integer_sequence<unsigned, 4>(), std::bind([&](auto const uSize, auto const uStepX)
				{
					for_each(std::make_integer_sequence<short, 4>(), std::bind([&](auto const uSize, auto const uStepX, auto const wAlpha)
					{
						test_sscal<decltype(uSize)::value, decltype(uStepX)::value, decltype(wAlpha)::value>();
					}, uSize, uStepX, std::placeholders::_1));
				}, uSize, std::placeholders::_1));
			});
		}
	};
}
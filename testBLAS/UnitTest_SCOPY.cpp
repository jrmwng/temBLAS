#include "stdafx.h"
#include "CppUnitTest.h"

extern "C"
{
#include "..\sharedBLAS\scopy.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace testBLAS
{		
	TEST_CLASS(testBLAS)
	{
		template <unsigned uSIZE, unsigned uSTEPX, unsigned uSTEPY>
		std::enable_if_t<(uSIZE && uSTEPX && uSTEPY)> test_scopy()
		{
			float arX[uSIZE * uSTEPX];
			{
				for (unsigned short uIndex = 0; arX + uIndex < std::end(arX); uIndex++)
				{
					arX[uIndex] = uIndex;
				}
			}
			float arYref[uSIZE * uSTEPY];
			float arYavx[uSIZE * uSTEPY];
			{
				std::fill(std::begin(arYref), std::end(arYref), std::numeric_limits<float>::quiet_NaN());
				std::fill(std::begin(arYavx), std::end(arYavx), std::numeric_limits<float>::quiet_NaN());
				SCOPY_ref(uSIZE, arX, uSTEPX, arYref, uSTEPY);
				SCOPY_avx(uSIZE, arX, uSTEPX, arYavx, uSTEPY);
			}
			for (unsigned uIndexY = 0; uIndexY < uSIZE * uSTEPY; uIndexY += uSTEPY)
			{
				if (arYavx[uIndexY] != arYref[uIndexY])
				{
					Assert::AreEqual(arYref[uIndexY], arYavx[uIndexY]);
				}
			}
		}
		template <unsigned uSIZE, unsigned uSTEPX, unsigned uSTEPY>
		std::enable_if_t<!(uSIZE && uSTEPX && uSTEPY)> test_scopy()
		{
		}
	public:
		TEST_METHOD(TestMethod_SCOPY)
		{
			for_each(std::make_integer_sequence<unsigned, 32>(), [&](auto const uSize)
			{
				for_each(std::make_integer_sequence<unsigned, 4>(), std::bind([&](auto const uSize, auto const uStepX)
				{
					for_each(std::make_integer_sequence<unsigned, 2>(), std::bind([&](auto const uSize, auto const uStepX, auto const uStepY)
					{
						test_scopy<decltype(uSize)::value, decltype(uStepX)::value, decltype(uStepY)::value>();
					}, uSize, uStepX, std::placeholders::_1));
				}, uSize, std::placeholders::_1));
			});
		}
	};
}
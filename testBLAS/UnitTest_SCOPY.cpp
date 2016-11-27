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
		void test_scopy()
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
	public:
		TEST_METHOD(TestMethod_SCOPY)
		{
			test_scopy<123, 1, 1>();
			test_scopy<123, 2, 1>();
			test_scopy<123, 3, 1>();
			test_scopy<128, 1, 1>();
			test_scopy<128, 2, 1>();
			test_scopy<128, 3, 1>();
		}
	};
}
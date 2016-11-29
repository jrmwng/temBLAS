#pragma once

/* Author: jrmwng @ 2016 */

// X <-- aX

void SSCAL_ref(unsigned uN, float rAlpha, float *prX, int nStepX);
void SSCAL_avx(unsigned uN, float rAlpha, float *prX, int nStepX);

#pragma once

/* Author: jrmwng @ 2016 */

// Y <-- X

void SCOPY_ref(unsigned uN, float const *prX, int nStepX, float *prY, int nStepY);
void SCOPY_avx(unsigned uN, float const *prX, int nStepX, float *prY, int nStepY);

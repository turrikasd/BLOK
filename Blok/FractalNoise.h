#pragma once

#include "anl.h"

class FractalNoise
{
public:
	FractalNoise();
	~FractalNoise();

	double Get(int x, int y, int z);

private:
	anl::CImplicitCombiner* combiner;
	anl::CImplicitAutoCorrect* noise;

	anl::CMWC4096 rnd;
	anl::CImplicitFractal* frac1;
	anl::CImplicitFractal* frac2;
};


#pragma once

#include "anl.h"

class FractalNoise
{
public:
	FractalNoise();
	~FractalNoise();

	double Get(int x, int y, int z);

private:
	anl::CImplicitCombiner* noise;

	anl::CMWC4096 rnd;
};


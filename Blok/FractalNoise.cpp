#include "FractalNoise.h"


FractalNoise::FractalNoise()
{
	rnd.setSeed(420);
	//rnd.setSeedTime();
	frac1 = new anl::CImplicitFractal(anl::HYBRIDMULTI, anl::GRADIENT, anl::LINEAR);
	frac2 = new anl::CImplicitFractal(anl::RIDGEDMULTI, anl::GRADIENT, anl::LINEAR);

	frac1->setSeed(rnd.get());
	frac2->setSeed(rnd.get());

	combiner = new anl::CImplicitCombiner(anl::MIN);
	combiner->setSource(0, frac1);
	combiner->setSource(1, frac2);

	noise = new anl::CImplicitAutoCorrect(0.0, 1.0);
	noise->setSource(combiner);
}


FractalNoise::~FractalNoise()
{
}

double FractalNoise::Get(int x, int y, int z)
{
	//return noise.get(0, 0, 0);
	return noise->get(double(x) / 100.0, double(y) / 100.0, double(z) / 100.0);
}
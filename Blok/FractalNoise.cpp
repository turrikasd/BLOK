#include "FractalNoise.h"
#include <iostream>
#include "World.h"

using namespace anl;

FractalNoise::FractalNoise()
{
	rnd.setSeed(420);

	/*CImplicitGradient* groundGradient = new CImplicitGradient();
	groundGradient->setGradient(0.0, 0.0, 0.0, 1.0);

	CImplicitFractal* lowlandShapeFrac = new CImplicitFractal(BILLOW, GRADIENT, QUINTIC);
	lowlandShapeFrac->setNumOctaves(2);
	lowlandShapeFrac->setFrequency(0.25);

	CImplicitAutoCorrect* lowLandAutocorrect = new CImplicitAutoCorrect(0.0, 1.0);
	lowLandAutocorrect->setSource(lowlandShapeFrac);

	CImplicitScaleOffset* lowLandScale = new CImplicitScaleOffset(0.125, -0.45);
	lowLandScale->setSource(lowLandAutocorrect);

	CImplicitScaleDomain* lowlandYScale = new CImplicitScaleDomain();
	lowlandYScale->setSource(lowLandScale);
	lowlandYScale->setYScale(0.0);

	CImplicitTranslateDomain* lowLandTerrain = new CImplicitTranslateDomain();
	lowLandTerrain->setSource(groundGradient);
	lowLandTerrain->setYAxisSource(lowlandYScale);

	// --------------

	CImplicitFractal* highLandShapeFrac = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	highLandShapeFrac->setNumOctaves(4);
	highLandShapeFrac->setFrequency(0.4);

	CImplicitAutoCorrect* highLandAutocorrect = new CImplicitAutoCorrect(-1, 1);
	highLandAutocorrect->setSource(highLandShapeFrac);

	CImplicitScaleOffset* highLandScale = new CImplicitScaleOffset(0.25, 0.0);
	highLandScale->setSource(highLandAutocorrect);

	CImplicitScaleDomain* highLandYScale = new CImplicitScaleDomain();
	highLandYScale->setSource(highLandScale);
	highLandYScale->setYScale(0.0);

	CImplicitTranslateDomain* highLandTerrain = new CImplicitTranslateDomain();
	highLandTerrain->setSource(groundGradient);
	highLandTerrain->setYAxisSource(highLandYScale);

	// -------------

	CImplicitFractal* mountainShapeFrac = new CImplicitFractal(RIDGEDMULTI, GRADIENT, QUINTIC);
	mountainShapeFrac->setNumOctaves(8);
	mountainShapeFrac->setFrequency(0.2);

	CImplicitAutoCorrect* mountainAutocorrect = new CImplicitAutoCorrect(-1, 1);
	mountainAutocorrect->setSource(mountainShapeFrac);

	CImplicitScaleOffset* mountainScale = new CImplicitScaleOffset(0.45, 0.15);
	mountainScale->setSource(mountainAutocorrect);

	CImplicitScaleDomain* mountainYScale = new CImplicitScaleDomain();
	mountainYScale->setSource(mountainScale);
	mountainYScale->setYScale(0.25);

	CImplicitTranslateDomain* mountainTerrain = new CImplicitTranslateDomain();
	mountainTerrain->setSource(groundGradient);
	mountainTerrain->setYAxisSource(mountainYScale);

	// ------------

	CImplicitFractal* terrainTypeFrac = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	terrainTypeFrac->setNumOctaves(3);
	terrainTypeFrac->setFrequency(0.125);

	CImplicitAutoCorrect* terrainAutocorrect = new CImplicitAutoCorrect(0.0, 1.0);
	terrainAutocorrect->setSource(terrainTypeFrac);

	CImplicitScaleDomain* terrainTypeYScale = new CImplicitScaleDomain();
	terrainTypeYScale->setSource(terrainAutocorrect);
	terrainTypeYScale->setYScale(0.0);

	CImplicitSelect* highLandMountainSelect = new CImplicitSelect();
	highLandMountainSelect->setControlSource(terrainTypeYScale);
	highLandMountainSelect->setLowSource(highLandTerrain);
	highLandMountainSelect->setHighSource(mountainTerrain);
	highLandMountainSelect->setThreshold(0.55);
	highLandMountainSelect->setFalloff(0.2);

	CImplicitSelect* highLandLowLandSelect = new CImplicitSelect();
	highLandLowLandSelect->setControlSource(terrainTypeYScale);
	highLandLowLandSelect->setLowSource(lowLandTerrain);
	highLandLowLandSelect->setHighSource(highLandMountainSelect);
	highLandLowLandSelect->setThreshold(0.25);
	highLandLowLandSelect->setFalloff(0.15);

	CImplicitSelect* groundSelect = new CImplicitSelect();
	groundSelect->setControlSource(highLandLowLandSelect);
	groundSelect->setLowSource(0.0);
	groundSelect->setHighSource(1.0);
	groundSelect->setThreshold(0.5);

	// ---------

	CImplicitFractal* caveFrac = new CImplicitFractal(RIDGEDMULTI, GRADIENT, QUINTIC);
	caveFrac->setNumOctaves(1);
	caveFrac->setFrequency(4);

	CImplicitBias* caveAttBias = new CImplicitBias(0.45);
	caveAttBias->setSource(highLandLowLandSelect);

	CImplicitCombiner* caveCom = new CImplicitCombiner(MULT);
	caveCom->setSource(0, caveFrac);
	caveCom->setSource(1, caveAttBias);

	CImplicitFractal* caveFrac2 = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	caveFrac2->setNumOctaves(6);
	caveFrac2->setFrequency(3);

	CImplicitScaleOffset* scaleo = new CImplicitScaleOffset(0.5, 0.0);
	scaleo->setSource(caveFrac2);

	CImplicitTranslateDomain* transdom = new CImplicitTranslateDomain();
	transdom->setSource(caveCom);
	transdom->setXAxisSource(scaleo);

	CImplicitSelect* sel = new CImplicitSelect();
	sel->setControlSource(transdom);
	sel->setLowSource(0.0);
	sel->setHighSource(1.0);
	sel->setThreshold(0.48);
	sel->setFalloff(0.0);

	noise = new CImplicitCombiner(MULT);
	noise->setSource(0, sel);
	noise->setSource(1, groundSelect);*/

	





/*


	CImplicitGradient* GroundGradient = new CImplicitGradient();
	GroundGradient->setGradient(0.0, 0.0, 0.0, 1.0);

	CImplicitFractal* GroundShape = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	GroundShape->setSeed(rnd.get());
	GroundShape->setNumOctaves(2);
	GroundShape->setFrequency(1.75);

	CImplicitScaleDomain* GroundShapeScale = new CImplicitScaleDomain(0.5, 0.5, 0.5);
	GroundShapeScale->setSource(GroundShape);

	CImplicitTranslateDomain* GroundTurb = new CImplicitTranslateDomain();
	GroundTurb->setSource(GroundGradient);
	GroundTurb->setYAxisSource(GroundShapeScale);

	CImplicitSelect* GroundBase = new CImplicitSelect();
	GroundBase->setControlSource(GroundTurb);
	GroundBase->setLowSource(-1.0);
	GroundBase->setHighSource(1.0);
	GroundBase->setThreshold(0.2);
	GroundBase->setFalloff(0.0);


	// ---------


	CImplicitFractal* CaveShape1 = new CImplicitFractal(RIDGEDMULTI, GRADIENT, QUINTIC);
	CaveShape1->setSeed(rnd.get());
	CaveShape1->setNumOctaves(1);
	CaveShape1->setFrequency(6.0);

	CImplicitSelect* CaveBase1 = new CImplicitSelect();
	CaveBase1->setControlSource(CaveShape1);
	CaveBase1->setLowSource(0.0);
	CaveBase1->setHighSource(1.0);
	CaveBase1->setThreshold(0.6);
	CaveBase1->setFalloff(0.0);

	CImplicitFractal* CaveShape2 = new CImplicitFractal(RIDGEDMULTI, GRADIENT, QUINTIC);
	CaveShape2->setSeed(1323);
	CaveShape2->setNumOctaves(1);
	CaveShape2->setFrequency(6.0);

	CImplicitSelect* CaveBase2 = new CImplicitSelect();
	CaveBase2->setControlSource(CaveShape2);
	CaveBase2->setLowSource(0.0);
	CaveBase2->setHighSource(1.0);
	CaveBase2->setThreshold(0.6);
	CaveBase2->setFalloff(0.0);

	CImplicitCombiner* CaveMult = new CImplicitCombiner(MULT);
	CaveMult->setSource(0, CaveBase1);
	CaveMult->setSource(1, CaveBase2);*/

	// -------------

/*	CImplicitFractal* CaveTurbX = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	CaveTurbX->setSeed(1001);
	CaveTurbX->setNumOctaves(3);
	CaveTurbX->setFrequency(3.0);

	CImplicitScaleDomain* CaveTurbXScale = new CImplicitScaleDomain(0.0, 0.0, 0.0);
	CaveTurbXScale->setSource(CaveTurbX);

	CImplicitFractal* CaveTurbY = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	CaveTurbY->setSeed(1201);
	CaveTurbY->setNumOctaves(3);
	CaveTurbY->setFrequency(3.0);

	CImplicitScaleDomain* CaveTurbYScale = new CImplicitScaleDomain(0.0, 0.0, 0.0);
	CaveTurbYScale->setSource(CaveTurbY);

	CImplicitFractal* CaveTurbZ = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	CaveTurbZ->setSeed(1301);
	CaveTurbZ->setNumOctaves(3);
	CaveTurbZ->setFrequency(3.0);

	CImplicitScaleDomain* CaveTurbZScale = new CImplicitScaleDomain(0.0, 0.0, 0.0);
	CaveTurbZScale->setSource(CaveTurbZ);

	CImplicitTranslateDomain* CaveTurb = new CImplicitTranslateDomain();
	CaveTurb->setSource(CaveMult);
	CaveTurb->setXAxisSource(CaveTurbXScale);
	CaveTurb->setYAxisSource(CaveTurbYScale);
	CaveTurb->setZAxisSource(CaveTurbZScale);

	CImplicitScaleOffset* CaveInvert = new CImplicitScaleOffset(-1.0, 1.0);
	CaveInvert->setSource(CaveMult);*/

	/*
	noise = new CImplicitCombiner(MULT);
	noise->setSource(0, GroundBase);
	noise->setSource(1, CaveMult);
	*/

	double Open = 1.0;
	double Dirt = 2.0;
	double Stone = 3.0;
	double SemiRare = 4.0;
	double Rare = 5.0;
	double Bedrock = 6.0;

	double SEMIRARE_DENSITY = 0.80;

	double RARE_GRADIENT_SCALE = 0.5;
	double RARE_DENSITY = 1.0;

	double DIRT_THRESHOLD = 1.03;

	double GROUND_YSCALE = 0.5;

	double CAVE_SIZE = 0.05;

	double BEDROCK_THRESHOLD = 1.98;

	CImplicitGradient* MainGradient = new CImplicitGradient();
	MainGradient->setGradient(0.0, 0.0, 0.0, 0.5);

	CImplicitScaleOffset* MainGradientRemap = new CImplicitScaleOffset(0.5, 0.5);
	MainGradientRemap->setSource(MainGradient);

	CImplicitFractal* SemiRareFBM = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	SemiRareFBM->setSeed(rnd.get());
	SemiRareFBM->setNumOctaves(4);
	SemiRareFBM->setFrequency(2.0);

	CImplicitScaleOffset* SemiRareFBMRemap = new CImplicitScaleOffset(0.5, 0.5);
	SemiRareFBMRemap->setSource(SemiRareFBM);

	CImplicitSelect* SemiRareSelect = new CImplicitSelect();
	SemiRareSelect->setControlSource(SemiRareFBMRemap);
	SemiRareSelect->setLowSource(Stone);
	SemiRareSelect->setHighSource(SemiRare);
	SemiRareSelect->setThreshold(SEMIRARE_DENSITY);
	SemiRareSelect->setFalloff(0.0);

	CImplicitFractal* RareFBM = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	RareFBM->setSeed(rnd.get() + 1);
	RareFBM->setNumOctaves(3);
	RareFBM->setFrequency(3.0);

	CImplicitScaleOffset* RareFBMRemap = new CImplicitScaleOffset(0.5, 0.5);
	RareFBMRemap->setSource(RareFBM);

	CImplicitScaleOffset* RareFBMScale = new CImplicitScaleOffset(RARE_GRADIENT_SCALE, 0.0);
	RareFBMScale->setSource(RareFBMRemap);

	CImplicitCombiner* RareMult = new CImplicitCombiner(MULT);
	RareMult->setSource(0, RareFBMScale);
	RareMult->setSource(1, MainGradientRemap);

	CImplicitScaleOffset* RareMultScale = new CImplicitScaleOffset(RARE_DENSITY, 0.0);
	RareMultScale->setSource(RareMult);

	CImplicitSelect* RareSelect = new CImplicitSelect();
	RareSelect->setControlSource(RareMultScale);
	RareSelect->setLowSource(SemiRareSelect);
	RareSelect->setHighSource(Rare);
	RareSelect->setThreshold(0.5);
	RareSelect->setFalloff(0.0);

	CImplicitSelect* DirtStoneSelect = new CImplicitSelect();
	DirtStoneSelect->setControlSource(MainGradientRemap);
	DirtStoneSelect->setLowSource(Dirt);
	DirtStoneSelect->setHighSource(RareSelect);
	DirtStoneSelect->setThreshold(DIRT_THRESHOLD);
	DirtStoneSelect->setFalloff(0.0);

	CImplicitSelect* GroundSelect = new CImplicitSelect();
	GroundSelect->setControlSource(MainGradientRemap);
	GroundSelect->setLowSource(Open);
	GroundSelect->setHighSource(DirtStoneSelect);
	GroundSelect->setThreshold(1.0);
	GroundSelect->setFalloff(0.0);


	CImplicitFractal* GroundShape = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	GroundShape->setSeed(rnd.get() + 2);
	GroundShape->setNumOctaves(2);
	GroundShape->setFrequency(2.0);

	CImplicitScaleOffset* GroundScale = new CImplicitScaleOffset(0.5, 0.0);
	GroundScale->setSource(GroundShape);

	CImplicitScaleDomain* GroundYScale = new CImplicitScaleDomain();
	GroundYScale->setSource(GroundScale);
	GroundYScale->setYScale(GROUND_YSCALE);

	CImplicitTranslateDomain* GroundTurb = new CImplicitTranslateDomain();
	GroundTurb->setSource(GroundSelect);
	GroundTurb->setYAxisSource(GroundYScale);

	// ---- CAVES
	CImplicitFractal* CaveShape1 = new CImplicitFractal(RIDGEDMULTI, GRADIENT, QUINTIC);
	CaveShape1->setSeed(rnd.get() + 3);
	CaveShape1->setNumOctaves(1);
	CaveShape1->setFrequency(2.0);

	CImplicitSelect* CaveBase1 = new CImplicitSelect();
	CaveBase1->setControlSource(CaveShape1);
	CaveBase1->setLowSource(0.0);
	CaveBase1->setHighSource(1.0);
	CaveBase1->setThreshold(1.0 - CAVE_SIZE);
	CaveBase1->setFalloff(0.0);

	CImplicitFractal* CaveShape2 = new CImplicitFractal(RIDGEDMULTI, GRADIENT, QUINTIC);
	CaveShape2->setSeed(rnd.get() + 4);
	CaveShape2->setNumOctaves(1);
	CaveShape2->setFrequency(2.0);

	CImplicitSelect* CaveBase2 = new CImplicitSelect();
	CaveBase2->setControlSource(CaveShape2);
	CaveBase2->setLowSource(0.0);
	CaveBase2->setHighSource(1.0);
	CaveBase2->setThreshold(1.0 - CAVE_SIZE);
	CaveBase2->setFalloff(0.0);

	CImplicitCombiner* CaveMult = new CImplicitCombiner(MULT);
	CaveMult->setSource(0, CaveBase1);
	CaveMult->setSource(1, CaveBase2);

	CImplicitFractal* CaveTurbFrac = new CImplicitFractal(FBM, GRADIENT, QUINTIC);
	CaveTurbFrac->setSeed(rnd.get() + 5);
	CaveTurbFrac->setNumOctaves(3);
	CaveTurbFrac->setFrequency(3.0);

	CImplicitTranslateDomain* CaveTurb = new CImplicitTranslateDomain();
	CaveTurb->setSource(CaveMult);
	CaveTurb->setXAxisSource(CaveTurbFrac);

	CImplicitScaleOffset* CaveInvert = new CImplicitScaleOffset(-1.0, 1.0);
	CaveInvert->setSource(CaveTurb);

	CImplicitSelect* CaveSelect = new CImplicitSelect();
	CaveSelect->setControlSource(CaveInvert);
	CaveSelect->setLowSource(Open);
	CaveSelect->setHighSource(GroundTurb);
	CaveSelect->setThreshold(0.5);
	CaveSelect->setFalloff(0.0);

	CImplicitSelect* BedrockSelect = new CImplicitSelect();
	BedrockSelect->setControlSource(MainGradient);
	BedrockSelect->setLowSource(CaveSelect);
	BedrockSelect->setHighSource(Bedrock);
	BedrockSelect->setThreshold(BEDROCK_THRESHOLD);
	BedrockSelect->setFalloff(0.0);

	noise = new CImplicitCombiner(MULT);
	noise->setSource(0, BedrockSelect);

	double d = noise->get(0.0, 0.0, 0.0);
	d = noise->get(0.0, 0.6, 0.0);
	d = 0;
}


FractalNoise::~FractalNoise()
{
}

double FractalNoise::Get(int x, int y, int z)
{
//	std::cout << noise->get((double)x, 0.0, (double)z) << std::endl;
	double yy = World::HEIGHT - y;
	return noise->get(double(x) / World::HEIGHT, double(yy) / World::HEIGHT, double(z) / World::HEIGHT);
}
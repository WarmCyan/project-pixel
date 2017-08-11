//*************************************************************
//  File: FunctionGenerator.cpp
//  Date created: 2/19/2017
//  Date edited: 8/10/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************

#ifndef FF_FUNCTION_GENERATOR_CPP
#define FF_FUNCTION_GENERATOR_CPP

#include "FunctionGenerator.h"

namespace dwl
{
	FFFunction* FunctionGenerator::GenerateFunction()
	{
		FFFunction* pFunction = new FFFunction();

		float fA = 0.0f;
		float fB = 0.0f;
		float fC = 0.0f;
		float fD = 0.0f;
		float fE = 0.0f;
		float fF = 0.0f;

		
		if (RandomFloat() > .5) { fA = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fB = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fC = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fD = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fE = RandomFloat() * 4 - 2; }
		if (RandomFloat() > .5) { fF = RandomFloat() * 4 - 2; }
		
		for (int i = 0; i <= 8; i++)
		{
			if (RandomFloat() > .5)
			{
				pFunction->SetVariationWeight(i, RandomFloat());
			}
		}

		pFunction->SetMatrixCoefficients({fA, fB, fC, fD, fE, fF});
		pFunction->SetWeight(RandomFloat());
		pFunction->SetColor(RandomFloat());

		return pFunction;
	}
	
	FFFunction* FunctionGenerator::GenerateSymmetryFunction(int iDegree)
	{
		FFFunction* pFunction = new FFFunction();

		float fA = (float)cos(iDegree*PI/180);
		float fB = (float)sin(iDegree*PI/180);
		float fC = 0.0f;
		float fD = -(float)sin(iDegree*PI/180);
		float fE = (float)cos(iDegree*PI/180);
		float fF = 0.0f;

		
		pFunction->SetVariationWeight(FFFunction::VAR_LINEAR, 1.0f);
		pFunction->SetMatrixCoefficients({fA, fB, fC, fD, fE, fF});
		
		pFunction->SetWeight(RandomFloat());
		pFunction->SetSymmetry(true);
		pFunction->SetColor(RandomFloat());

		return pFunction;
	}
	
	float FunctionGenerator::RandomFloat()
	{
		random_device rd;
		srand(rd());
		float fRandom = ((float) rand()) / (float) RAND_MAX;

		return fRandom;
	}

	// NOTE: bounds are inclusive
	float FunctionGenerator::RandomInt(int iLow, int iHigh) { return (int)round(RandomFloat() * iHigh - iLow); }

	void FunctionGenerator::GenerateFractalFunctionSet(FlameFractal* pFractal)
	{
		// How many functions?
		
		// For each function
		//	Function variation decision thresh?

		// Symmetry?
		
		// current function weight sum in fractal?

		// Symmetry count?
	}

}

#endif

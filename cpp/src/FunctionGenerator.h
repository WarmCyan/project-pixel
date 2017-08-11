//*************************************************************
//  File: FunctionGenerator.h
//  Date created: 2/19/2017
//  Date edited: 8/10/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************

#ifndef FF_FUNCTION_GENERATOR_H
#define FF_FUNCTION_GENERATOR_H

#include "Function.h"
#include "FlameFractal.h"
#include <random>
#include <cmath>

#define PI 3.14159265

namespace dwl
{
	class FunctionGenerator
	{
		private:	
			float RandomFloat();
			int RandomInt(int iLow, int iHigh);
			
		public:
			FFFunction* GenerateFunction(float fDecisionThreshold);
			FFFunction* GenerateSymmetryFunction(int iDegree);
			void GenerateFractalFunctionSet(FlameFractal* pFractal);
	};
}


#endif

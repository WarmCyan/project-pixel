//*************************************************************
//  File: Function.h
//  Date created: 1/28/2017
//  Date edited: 8/11/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************

#ifndef FF_FUNCTION_H
#define FF_FUNCTION_H

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

//#include "lib/pugixml.hpp"
#include <pugixml.hpp>

#define PI 3.14159265
#define EPS (1e-8)

using namespace std;
using namespace pugi;

namespace dwl
{
	class FFFunction
	{
		private:
			float m_fColor;
			float m_fWeight;

			bool m_bIsSymmetry;

			float m_fTempX;
			float m_fTempY;

			float m_fResultX;
			float m_fResultY;

			float m_fR;
			float m_fTheta;

			float m_fOmega;

			vector<float> m_vMatrixCoefficients;
			vector<float> m_vPostCoefficients;
			vector<float> m_vVariationWeights;
			//float* m_vMatrixCoefficients;
			//float* m_vVariationWeights;

			float Calc_R(float fX, float fY);
			float Calc_Theta(float fX, float fY);
			float Calc_Phi(float fX, float fY);
			
			// variation functions
			void Var_Linear(float fX, float fY);
			void Var_Sinusoidal(float fX, float fY);
			void Var_Spherical(float fX, float fY);
			void Var_Swirl(float fX, float fY);
			void Var_Horseshoe(float fX, float fY);
			void Var_Polar(float fX, float fY);
			void Var_Handkerchief(float fX, float fY);
			void Var_Heart(float fX, float fY);
			void Var_Disc(float fX, float fY);
			void Var_Spiral(float fX, float fY);
			void Var_Hyperbolic(float fX, float fY);
			void Var_Diamond(float fX, float fY);
			void Var_Ex(float fX, float fY);
			void Var_Julia(float fX, float fY);
			void Var_Bent(float fX, float fY);
			void Var_Waves(float fX, float fY);
			void Var_Fisheye(float fX, float fY);
			void Var_Popcorn(float fX, float fY);
			void Var_Exponential(float fX, float fY);
			void Var_Power(float fX, float fY);
			void Var_Cosine(float fX, float fY);
			void Var_Rings(float fX, float fY);
			
		public:

			static const int NUM_VARIATIONS=21;

			static const int VAR_LINEAR = 0;
			static const int VAR_SINUSOIDAL = 1;
			static const int VAR_SPHERICAL = 2;
			static const int VAR_SWIRL = 3;
			static const int VAR_HORSESHOE = 4;
			static const int VAR_POLAR = 5;
			static const int VAR_HANDKERCHIEF = 6;
			static const int VAR_HEART = 7;
			static const int VAR_DISC = 8;
			static const int VAR_SPIRAL = 9;
			static const int VAR_HYPERBOLIC = 10;
			static const int VAR_DIAMOND = 11;
			static const int VAR_EX = 12;
			static const int VAR_JULIA = 13;
			static const int VAR_BENT = 14;
			static const int VAR_WAVES = 15;
			static const int VAR_FISHEYE = 16;
			static const int VAR_POPCORN = 17;
			static const int VAR_EXPONENTIAL = 18;
			static const int VAR_POWER = 19;
			static const int VAR_COSINE = 20;
			static const int VAR_RINGS = 21;
			
			FFFunction();
			void Run(float fX, float fY);

			void SetSymmetry(bool bIsSymmetry) { m_bIsSymmetry = bIsSymmetry; }
			bool IsSymmetry() { return m_bIsSymmetry; }

			float GetColor() { return m_fColor; }
			void SetColor(float fColor) { m_fColor = fColor; }

			vector<float> GetVariationWeights() { return m_vVariationWeights; }
			void SetVariationWeights(vector<float> aVariationWeights) { m_vVariationWeights = aVariationWeights; }
			void SetVariationWeight(int iVariation, float fWeight) { m_vVariationWeights[iVariation] = fWeight; }

			void SetMatrixCoefficients(vector<float> aMatrixCoefficients) { m_vMatrixCoefficients = aMatrixCoefficients; }
			void SetPostCoefficients(vector<float> aPostCoefficients) { m_vPostCoefficients = aPostCoefficients; }

			float GetWeight() { return m_fWeight; }
			void SetWeight(float fWeight) { m_fWeight = fWeight; }

			float GetResultX() { return m_fResultX; }
			float GetResultY() { return m_fResultY; }

			string FunctionInfo();	 

			xml_document* GetFunctionXML();
			void LoadFromXML(xml_node pNode);
	};
}
#endif // FF_FUNCTION_H

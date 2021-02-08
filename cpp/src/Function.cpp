//*************************************************************
//  File: Function.cpp
//  Date created: 1/28/2017
//  Date edited: 8/11/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************

#ifndef FF_FUNCTION_CPP
#define FF_FUNCTION_CPP

#include "Function.h"

using namespace std;

namespace dwl
{
	FFFunction::FFFunction()
	{
		m_fColor = 0.0f;
		m_fWeight = 1.0f;

		m_bIsSymmetry = false;

		m_fResultX = 0.0f;
		m_fResultY = 0.0f;
		m_fTempX = 0.0f;
		m_fTempY = 0.0f;

		m_fR = 0.0f;
		m_fTheta = 0.0f;

		// other parameters
		m_fOmega = 0.0f;

		//float aMatrixCoefficients
		m_vMatrixCoefficients = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }; // linear by default (ax + by + c, dx + ey + f)
		m_vPostCoefficients = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }; // linear by default (alphax + betay + gamma, deltax + epsilony + zeta) [THIS IS THE POST TRANSFORM]
		//m_vVariationWeights = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		m_vVariationWeights = {};
		for (int i = 0; i <= 18; i++) { m_vVariationWeights.push_back(0.0f); }
	}

	void FFFunction::Run(float fX, float fY)
	{
		//cout << "running" << endl; // DEBUG
		//float fResultX = 0.0f;
		//float fResultY = 0.0f;
		
		m_fResultX = 0.0f;
		m_fResultY = 0.0f;

		/*float fA = m_vMatrixCoefficients[0];
		float fB = m_vMatrixCoefficients[1];
		float fC = m_vMatrixCoefficients[2];
		float fD = m_vMatrixCoefficients[3];
		float fE = m_vMatrixCoefficients[4];
		float fF = m_vMatrixCoefficients[5];*/

		float fTerm1 = m_vMatrixCoefficients[0] * fX + m_vMatrixCoefficients[1] * fY + m_vMatrixCoefficients[2];
		float fTerm2 = m_vMatrixCoefficients[3] * fX + m_vMatrixCoefficients[4] * fY + m_vMatrixCoefficients[5];
		
		//m_fR = Calc_R(fX, fY);
		//m_fTheta = Calc_Theta(fX, fY);
		
		// I THINK this version is correct
		m_fR = Calc_R(fTerm1, fTerm2);
		m_fTheta = Calc_Theta(fTerm1, fTerm2);
		
		if (m_vVariationWeights[VAR_LINEAR] > 0)
		{
			Var_Linear(fTerm1, fTerm2);
			//cout << "Linear:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_LINEAR];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_LINEAR];
		}
		//cout << "\tPast linear " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_SINUSOIDAL] > 0)
		{
			Var_Sinusoidal(fTerm1, fTerm2);
			//cout << "Sinusoidal:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SINUSOIDAL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SINUSOIDAL];
		}
		//cout << "\tPast sinusoidal " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_SPHERICAL] > 0)
		{
			Var_Spherical(fTerm1, fTerm2);
			//cout << "Spherical:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SPHERICAL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SPHERICAL];
		}
		//cout << "\tPast spherical " << m_fResultX << "," << m_fResultY << endl; // DEBUG

		if (m_vVariationWeights[VAR_SWIRL] > 0)
		{
			Var_Swirl(fTerm1, fTerm2);
			//cout << "Swirl:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SWIRL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SWIRL];
		}
		//cout << "\tPast swirl " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_HORSESHOE] > 0)
		{
			Var_Horseshoe(fTerm1, fTerm2);
			//cout << "Horseshoe:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HORSESHOE];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HORSESHOE];
		}
		//cout << "\tPast horseshoe " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_POLAR] > 0)
		{
			Var_Polar(fTerm1, fTerm2);
			//cout << "Polar:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_POLAR];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_POLAR];
		}
		//cout << "\tPast polar " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_HANDKERCHIEF] > 0)
		{
			Var_Handkerchief(fTerm1, fTerm2);
			//cout << "Handkerchief:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HANDKERCHIEF];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HANDKERCHIEF];
		}
		//cout << "\tPast hankderchief " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_HEART] > 0)
		{
			Var_Heart(fTerm1, fTerm2);
			//cout << "Heart:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HEART];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HEART];
		}
		//cout << "\tPast heart " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_DISC] > 0)
		{
			Var_Disc(fTerm1, fTerm2);
			//cout << "Disc:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_DISC];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_DISC];
		}
		//cout << "\tPast disc " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_SPIRAL] > 0)
		{
			Var_Spiral(fTerm1, fTerm2);
			//cout << "Spiral:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SPIRAL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SPIRAL];
		}
		//cout << "\tPast spiral " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_HYPERBOLIC] > 0)
		{
			Var_Hyperbolic(fTerm1, fTerm2);
			//cout << "Hyperbolic::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HYPERBOLIC];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HYPERBOLIC];
		}
		//cout << "\tPast hyperbolic " << m_fResultX << "," << m_fResultY << endl; // DEBUG

		if (m_vVariationWeights[VAR_DIAMOND] > 0)
		{
			Var_Diamond(fTerm1, fTerm2);
			//cout << "Diamond::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_DIAMOND];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_DIAMOND];
		}
		//cout << "\tPast diamond " << m_fResultX << "," << m_fResultY << endl; // DEBUG

		if (m_vVariationWeights[VAR_EX] > 0)
		{
			Var_Ex(fTerm1, fTerm2);
			//cout << "Ex::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_EX];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_EX];
		}
		//cout << "\tPast ex " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_JULIA] > 0)
		{
			Var_Julia(fTerm1, fTerm2);
			//cout << "Julia::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_JULIA];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_JULIA];
		}
		//cout << "\tPast julia " << m_fResultX << "," << m_fResultY << endl; // DEBUG

		if (m_vVariationWeights[VAR_BENT] > 0)
		{
			Var_Bent(fTerm1, fTerm2);
			//cout << "bent::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_BENT];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_BENT];
		}
		//cout << "\tPast bent " << m_fResultX << "," << m_fResultY << endl; // DEBUG
	
		if (m_vVariationWeights[VAR_WAVES] > 0)
		{
			Var_Waves(fTerm1, fTerm2);
			//cout << "waves::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_WAVES];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_WAVES];
		}
		//cout << "\tPast waves " << m_fResultX << "," << m_fResultY << endl; // DEBUG
	
		if (m_vVariationWeights[VAR_FISHEYE] > 0)
		{
			Var_Fisheye(fTerm1, fTerm2);
			//cout << "fisheye::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_FISHEYE];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_FISHEYE];
		}
		//cout << "\tPast fisheye " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_POPCORN] > 0)
		{
			Var_Popcorn(fTerm1, fTerm2);
			//cout << "popcorn::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_POPCORN];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_POPCORN];
		}
		//cout << "\tPast popcorn " << m_fResultX << "," << m_fResultY << endl; // DEBUG
		
		if (m_vVariationWeights[VAR_EXPONENTIAL] > 0)
		{
			Var_Exponential(fTerm1, fTerm2);
			//cout << "exponential::" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_EXPONENTIAL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_EXPONENTIAL];
		}
		//cout << "\tPast exponential " << m_fResultX << "," << m_fResultY << endl; // DEBUG
	
		
	
		// apply the post transform (section 3.1 in paper)
		m_fTempX = m_fResultX;	
		m_fTempY = m_fResultY;	
		m_fResultX = m_vPostCoefficients[0] * m_fTempX + m_vPostCoefficients[1] * m_fTempY + m_vPostCoefficients[2];
		m_fResultY = m_vPostCoefficients[3] * m_fTempX + m_vPostCoefficients[4] * m_fTempY;

		//return { fResultX, fResultY };
	}

	float FFFunction::Calc_R(float fX, float fY) { return sqrt(pow(fX, 2) + pow(fY, 2)); }
	float FFFunction::Calc_Theta(float fX, float fY) 
	{ 
		float fDiv = 0.0f;
		if (fY != 0) { fDiv = fX/fY; }
		return atan(fDiv); 
	}
	float FFFunction::Calc_Phi(float fX, float fY) 
	{ 
		float fDiv = 0.0f;
		if (fX != 0) { fDiv = fY/fX; }
		return atan(fDiv); 
	}

	// variations
	
	void FFFunction::Var_Linear(float fX, float fY) 
	{ 
		m_fTempX = fX;
		m_fTempY = fY;
	}
	void FFFunction::Var_Sinusoidal(float fX, float fY) 
	{ 
		m_fTempX = sin(fX);
		m_fTempY = sin(fY);
	}
	void FFFunction::Var_Spherical(float fX, float fY) 
	{
		//float fR = Calc_R(fX, fY);
		float fCoef = 0.0f;
		if (m_fR != 0) { fCoef = 1 / (pow(m_fR, 2)); }

		m_fTempX = fX * fCoef;
		m_fTempY = fY * fCoef;
	}
	void FFFunction::Var_Swirl(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		float fRSquared = pow(m_fR, 2);
		float fcosRSquared = cos(fRSquared);
		float fsinRSquared = sin(fRSquared);

		m_fTempX = (fX * fsinRSquared) - (fY * fcosRSquared);
		m_fTempY = (fX * fcosRSquared) + (fY * fsinRSquared);
	}
	void FFFunction::Var_Horseshoe(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		float fInverseR = 1 / m_fR;
		if (m_fR == 0.0f) { fInverseR = 0.0f; } // NOTE: how to avoid getting nan when 0,0
		//float fInverseR = 1 / m_fR;

		m_fTempX = fInverseR * (fX - fY) * (fX + fY);
		m_fTempY = fInverseR * 2 * fX * fY;
	}
	void FFFunction::Var_Polar(float fX, float fY)
	{
		//m_fTempX = Calc_Theta(fX, fY) / PI;
		//m_fTempY = Calc_R(fX, fY) - 1;
		m_fTempX = m_fTheta / PI;
		m_fTempY = m_fR - 1;
	}
	void FFFunction::Var_Handkerchief(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		//float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = m_fR * sin(m_fTheta + m_fR);
		m_fTempY = m_fR * cos(m_fTheta - m_fR);
	}
	void FFFunction::Var_Heart(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		//float fTheta = Calc_Theta(fX, fY);

		m_fTempX = m_fR * sin(m_fR * m_fTheta);
		m_fTempY = m_fR * -1 * cos(m_fR * m_fTheta);
	}
	void FFFunction::Var_Disc(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		//float fCoef = Calc_Theta(fX, fY) / PI;
		float fCoef = m_fTheta / PI;
		
		m_fTempX = fCoef * sin(PI * m_fR);
		m_fTempY = fCoef * cos(PI * m_fR);
	}
	void FFFunction::Var_Spiral(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		//float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = (1 / m_fR) * (cos(m_fTheta) + sin(m_fR));
		m_fTempY = (1 / m_fR) * (sin(m_fTheta) - cos(m_fR));
	}
	void FFFunction::Var_Hyperbolic(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		float fR = m_fR;
		if (fR == 0) { fR = 1.0f; } // NOTE: how to avoid inf issues when 0,0
		float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = (1 / fR) * (cos(m_fTheta) + sin(m_fR));
		m_fTempY = (1 / fR) * (sin(m_fTheta) - cos(m_fR));
	}
	void FFFunction::Var_Diamond(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		//float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = sin(m_fTheta) * cos(m_fR);;
		m_fTempY = cos(m_fTheta) * sin(m_fR);
	}
	void FFFunction::Var_Ex(float fX, float fY)
	{
		//float fR = Calc_R(fX, fY);
		//float fTheta = Calc_Theta(fX, fY);
		
		float fP0 = sin(m_fTheta + m_fR);
		float fP1 = cos(m_fTheta - m_fR);
			
		m_fTempX = m_fR * (pow(fP0, 3) + pow(fP1, 3));
		m_fTempY = m_fR * (pow(fP0, 3) - pow(fP1, 3));
	} 
	void FFFunction::Var_Julia(float fX, float fY)
	{
		float fR = sqrt(m_fR);

		m_fTempX = fR * (cos(m_fTheta / 2 + m_fOmega));
		m_fTempY = fR * (sin(m_fTheta / 2 + m_fOmega));
	}
	void FFFunction::Var_Bent(float fX, float fY)
	{
		if (fX >= 0 && fY >= 0)
		{
			m_fTempX = fX;
			m_fTempY = fY;
		}
		else if (fX < 0 && fY >= 0)
		{
			m_fTempX = 2 * fX;
			m_fTempY = fY;
		}
		else if (fX >= 0 && fY < 0)
		{
			m_fTempX = fX;
			m_fTempY = fY / 2;
		}
		else // x < 0 y < 0
		{
			m_fTempX = 2 * fX;
			m_fTempY = fY / 2;
		}
	}
	void FFFunction::Var_Waves(float fX, float fY)
	{
		// TODO: these could be precalculated before function is ever used?
		float denom1;
		if (m_vMatrixCoefficients[2] == 0) 
		{ 
			m_fTempX = fX + m_vMatrixCoefficients[1];
		}
		else 
		{ 
			denom1 = pow(m_vMatrixCoefficients[2], 2); 
			m_fTempX = fX + m_vMatrixCoefficients[1] * sin(fY / denom1);
		}
		
		float denom2;
		if (m_vMatrixCoefficients[5] == 0) 
		{
			m_fTempY = fY + m_vMatrixCoefficients[4];
		}
		else
		{
			denom2 = pow(m_vMatrixCoefficients[5], 2);
			m_fTempY = fY + m_vMatrixCoefficients[4] * sin(fX / denom2);
		}
		
	}
	void FFFunction::Var_Fisheye(float fX, float fY)
	{
		float fCoef = 2 / (m_fR + 1);
		
		m_fTempX = fCoef * fY;
		m_fTempY = fCoef * fX;
	}
	void FFFunction::Var_Popcorn(float fX, float fY)
	{
		m_fTempX = fX + m_vMatrixCoefficients[2] * sin(tan(3 * fY));
		m_fTempY = fY + m_vMatrixCoefficients[5] * sin(tan(3 * fX));
	}
	void FFFunction::Var_Exponential(float fX, float fY)
	{
		// bad hack, did this because segfaults if not
		if (fX > 2) { fX = 2; }
		else if (fX < -2) { fX = -2; }
		float fCoef = exp(fX - 1);
		
		m_fTempX = fCoef * cos(PI * fY);
		m_fTempY = fCoef * sin(PI * fY);
	}


	string FFFunction::FunctionInfo()
	{
		return "This is a function...";
	}

	xml_document* FFFunction::GetFunctionXML()
	{
		cout << "Getting function xml..." << endl;
		//cout << "VariationWeights size first: " << m_vVariationWeights.size() << endl;
			
		xml_document* pFunctionDoc = new xml_document();

		xml_node pFunctionNode = pFunctionDoc->append_child("Function");
		pFunctionNode.append_attribute("Weight") = m_fWeight;
		pFunctionNode.append_attribute("Color") = m_fColor;

		pFunctionNode.append_attribute("IsSymmetry") = m_bIsSymmetry;

		pFunctionNode.append_attribute("A") = m_vMatrixCoefficients[0];
		pFunctionNode.append_attribute("B") = m_vMatrixCoefficients[1];
		pFunctionNode.append_attribute("C") = m_vMatrixCoefficients[2];
		pFunctionNode.append_attribute("D") = m_vMatrixCoefficients[3];
		pFunctionNode.append_attribute("E") = m_vMatrixCoefficients[4];
		pFunctionNode.append_attribute("F") = m_vMatrixCoefficients[5];

		//cout << "Made it to post coefficients" << endl;
		/*cout << m_vPostCoefficients[0] << endl;
		cout << m_vPostCoefficients[1] << endl;
		cout << m_vPostCoefficients[2] << endl;
		cout << m_vPostCoefficients[3] << endl;
		cout << m_vPostCoefficients[4] << endl;
		cout << m_vPostCoefficients[5] << endl;*/
		
		
		pFunctionNode.append_attribute("alpha") = m_vPostCoefficients[0];
		pFunctionNode.append_attribute("beta") = m_vPostCoefficients[1];
		pFunctionNode.append_attribute("gamma") = m_vPostCoefficients[2];
		pFunctionNode.append_attribute("delta") = m_vPostCoefficients[3];
		pFunctionNode.append_attribute("epsilon") = m_vPostCoefficients[4];
		pFunctionNode.append_attribute("zeta") = m_vPostCoefficients[5];

		//cout << "Made it past post" << endl;

		xml_node pVariationsNode = pFunctionNode.append_child("VariationWeights");
		//cout << "VariationWeights declared" << endl;

		//cout << "VariationWeights size: " << m_vVariationWeights.size() << endl;
		for (int i = 0; i < m_vVariationWeights.size(); i++)
		{
			//cout << i << endl;
			xml_node pVariationNode = pVariationsNode.append_child("VariationWeight");
			pVariationNode.append_attribute("Variation") = i;
			pVariationNode.append_attribute("Value") = m_vVariationWeights[i];
		}

		//cout << "GeFunctionXML is fine" << endl;

		return pFunctionDoc;
	}

	// assumes passing in literal <Function>
	void FFFunction::LoadFromXML(xml_node pNode)
	{
		cout << "Loading function..." << endl;
		
		m_fWeight = pNode.attribute("Weight").as_float();
		m_fColor = pNode.attribute("Color").as_float();

		m_bIsSymmetry = pNode.attribute("IsSymmetry").as_bool();

		// load matrix coefficients
		m_vMatrixCoefficients[0] = pNode.attribute("A").as_float();
		m_vMatrixCoefficients[1] = pNode.attribute("B").as_float();
		m_vMatrixCoefficients[2] = pNode.attribute("C").as_float();
		m_vMatrixCoefficients[3] = pNode.attribute("D").as_float();
		m_vMatrixCoefficients[4] = pNode.attribute("E").as_float();
		m_vMatrixCoefficients[5] = pNode.attribute("F").as_float();

		// load post transform coefficients if they exist
		xml_attribute attr;
		if (attr = pNode.attribute("alpha")) { m_vPostCoefficients[0] = attr.as_float(); }
		if (attr = pNode.attribute("beta")) { m_vPostCoefficients[1] = attr.as_float(); }
		if (attr = pNode.attribute("gamma")) { m_vPostCoefficients[2] = attr.as_float(); }
		if (attr = pNode.attribute("delta")) { m_vPostCoefficients[3] = attr.as_float(); }
		if (attr = pNode.attribute("epsilon")) { m_vPostCoefficients[4] = attr.as_float(); }
		if (attr = pNode.attribute("zeta")) { m_vPostCoefficients[5] = attr.as_float(); }

		int iIndex = 0;
		for (xml_node pVariationNode = pNode.child("VariationWeights").child("VariationWeight"); pVariationNode; pVariationNode = pVariationNode.next_sibling("VariationWeight"))
		{
			m_vVariationWeights[pVariationNode.attribute("Variation").as_int()] = pVariationNode.attribute("Value").as_float();
			iIndex++;
		}
	}
}

#endif // FF_FUNCTION_CPP

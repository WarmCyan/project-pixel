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

		//float aMatrixCoefficients
		m_vMatrixCoefficients = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }; // linear by default (ax + by + c, dx + ey + f)
		m_vVariationWeights = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
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
		
		m_fR = Calc_R(fX, fY);
		m_fTheta = Calc_Theta(fX, fY);
		
		if (m_vVariationWeights[VAR_LINEAR] > 0)
		{
			Var_Linear(fTerm1, fTerm2);
			//cout << "Linear:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_LINEAR];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_LINEAR];
		}
		
		if (m_vVariationWeights[VAR_SINUSOIDAL] > 0)
		{
			Var_Sinusoidal(fTerm1, fTerm2);
			//cout << "Sinusoidal:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SINUSOIDAL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SINUSOIDAL];
		}
		
		if (m_vVariationWeights[VAR_SPHERICAL] > 0)
		{
			Var_Spherical(fTerm1, fTerm2);
			//cout << "Spherical:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SPHERICAL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SPHERICAL];
		}

		if (m_vVariationWeights[VAR_SWIRL] > 0)
		{
			Var_Swirl(fTerm1, fTerm2);
			//cout << "Swirl:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SWIRL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SWIRL];
		}
		
		if (m_vVariationWeights[VAR_HORSESHOE] > 0)
		{
			Var_Horseshoe(fTerm1, fTerm2);
			//cout << "Horseshoe:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HORSESHOE];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HORSESHOE];
		}
		
		if (m_vVariationWeights[VAR_POLAR] > 0)
		{
			Var_Polar(fTerm1, fTerm2);
			//cout << "Polar:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_POLAR];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_POLAR];
		}
		
		if (m_vVariationWeights[VAR_HANDKERCHIEF] > 0)
		{
			Var_Handkerchief(fTerm1, fTerm2);
			//cout << "Handkerchief:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HANDKERCHIEF];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HANDKERCHIEF];
		}
		
		if (m_vVariationWeights[VAR_HEART] > 0)
		{
			Var_Heart(fTerm1, fTerm2);
			//cout << "Heart:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HEART];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HEART];
		}
		
		if (m_vVariationWeights[VAR_DISC] > 0)
		{
			Var_Disc(fTerm1, fTerm2);
			//cout << "Disc:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_DISC];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_DISC];
		}
		
		if (m_vVariationWeights[VAR_SPIRAL] > 0)
		{
			Var_Spiral(fTerm1, fTerm2);
			//cout << "Spiral:" << m_fTempX << "," << m_fTempY << endl; // DEBUG
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_SPIRAL];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_SPIRAL];
		}
		
		if (m_vVariationWeights[VAR_HYPERBOLIC] > 0)
		{
			Var_Hyperbolic(fTerm1, fTerm2);
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_HYPERBOLIC];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_HYPERBOLIC];
		}

		if (m_vVariationWeights[VAR_DIAMOND] > 0)
		{
			Var_Diamond(fTerm1, fTerm2);
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_DIAMOND];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_DIAMOND];
		}

		if (m_vVariationWeights[VAR_EX] > 0)
		{
			Var_Ex(fTerm1, fTerm2);
			m_fResultX += m_fTempX * m_vVariationWeights[VAR_EX];
			m_fResultY += m_fTempY * m_vVariationWeights[VAR_EX];
		}

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
		float fR = Calc_R(fX, fY);
		float fCoef = 1 / (pow(fR, 2));

		m_fTempX = fX * fCoef;
		m_fTempY = fY * fCoef;
	}
	void FFFunction::Var_Swirl(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fRSquared = pow(fR, 2);
		float fcosRSquared = cos(fRSquared);
		float fsinRSquared = sin(fRSquared);

		m_fTempX = (fX * fsinRSquared) - (fY * fcosRSquared);
		m_fTempY = (fX * fcosRSquared) + (fY * fsinRSquared);
	}
	void FFFunction::Var_Horseshoe(float fX, float fY)
	{
		float fInverseR = 1 / Calc_R(fX, fY);
		//float fInverseR = 1 / m_fR;

		m_fTempX = fInverseR * (fX - fY) * (fX + fY);
		m_fTempY = fInverseR * 2 * fX * fY;
	}
	void FFFunction::Var_Polar(float fX, float fY)
	{
		m_fTempX = Calc_Theta(fX, fY) / PI;
		m_fTempY = Calc_R(fX, fY) - 1;
		//m_fTempX = m_fTheta / PI;
		//m_fTempY = m_fR - 1;
	}
	void FFFunction::Var_Handkerchief(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = fR * sin(fTheta + fR);
		m_fTempY = fR * cos(fTheta - fR);
	}
	void FFFunction::Var_Heart(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fTheta = Calc_Theta(fX, fY);

		m_fTempX = fR * sin(fR * fTheta);
		m_fTempY = fR * -1 * cos(fR * fTheta);
	}
	void FFFunction::Var_Disc(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fCoef = Calc_Theta(fX, fY) / PI;
		//float fCoef = m_fTheta / PI;
		
		m_fTempX = fCoef * sin(PI * fR);
		m_fTempY = fCoef * cos(PI * fR);
	}
	void FFFunction::Var_Spiral(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = (1 / fR) * (cos(fTheta) + sin(fR));
		m_fTempY = (1 / fR) * (sin(fTheta) - cos(fR));
	}
	void FFFunction::Var_Hyperbolic(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = (1 / fR) * (cos(fTheta) + sin(fR));
		m_fTempY = (1 / fR) * (sin(fTheta) - cos(fR));
	}
	void FFFunction::Var_Diamond(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fTheta = Calc_Theta(fX, fY);
		
		m_fTempX = sin(fTheta) * cos(fR);;
		m_fTempY = cos(fTheta) * sin(fR);
	}
	void FFFunction::Var_Ex(float fX, float fY)
	{
		float fR = Calc_R(fX, fY);
		float fTheta = Calc_Theta(fX, fY);
		
		float fP0 = sin(fTheta + fR);
		float fP1 = cos(fTheta - fR);
			
		m_fTempX = fR * (pow(fP0, 3) + pow(fP1, 3));
		m_fTempY = fR * (pow(fP0, 3) - pow(fP1, 3));
	}

	string FFFunction::FunctionInfo()
	{
		return "This is a function...";
	}

	xml_document* FFFunction::GetFunctionXML()
	{
		cout << "Getting function xml..." << endl;
			
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

		xml_node pVariationsNode = pFunctionNode.append_child("VariationWeights");

		for (int i = 0; i < m_vVariationWeights.size(); i++)
		{
			xml_node pVariationNode = pVariationsNode.append_child("VariationWeight");
			pVariationNode.append_attribute("Variation") = i;
			pVariationNode.append_attribute("Value") = m_vVariationWeights[i];
		}

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

		int iIndex = 0;
		for (xml_node pVariationNode = pNode.child("VariationWeights").child("VariationWeight"); pVariationNode; pVariationNode = pVariationNode.next_sibling("VariationWeight"))
		{
			m_vVariationWeights[pVariationNode.attribute("Variation").as_int()] = pVariationNode.attribute("Value").as_float();
			iIndex++;
		}
	}
}

#endif // FF_FUNCTION_CPP

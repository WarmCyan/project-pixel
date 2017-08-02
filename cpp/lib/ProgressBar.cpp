//*************************************************************
//  File: ProgressBar.cpp
//  Date created: 2/9/2017
//  Date edited: 7/27/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: Class for displaying task progress on a single line
//*************************************************************

#ifndef PROGBAR_CPP
#define PROGBAR_CPP

#include "ProgressBar.h"

namespace dwl
{
	ProgressBar::ProgressBar(int iTotal, int iSize)
	{
		m_iTotal = iTotal;
		m_iSize = iSize;
		m_iTicks = 0;
		m_iPrevTickCount = 0;

		// account for extra characters
		m_iSize -= 6;

		// determine step size
		m_fStepSize = (float)m_iSize / (float)m_iTotal;

		m_iSize = ceil(m_fStepSize * iTotal);

		this->Update(0);
	}

	void ProgressBar::Update(int iCount)
	{
		m_iTicks = 0;
		int iTickCount = (int)(ceil(iCount * m_fStepSize));

		if (iTickCount <= m_iPrevTickCount && iCount < m_iTotal) { return; } // avoid unnecessary rerendering
		m_iPrevTickCount = iTickCount;
		
		int iPercentage = ceil(((float)iCount / (float)m_iTotal)*100);
		
		// print percentage
		cout << "\r" << iPercentage << "%";
		if (iPercentage < 10) { cout << "  "; }
		else if (iPercentage < 100) { cout << " "; }
		
		cout << m_sEndCharacter;

		while (m_iTicks < iTickCount)
		{
			cout << m_sTickCharacter;
			m_iTicks++;
		}
		while (m_iTicks < m_iSize)
		{
			cout << " ";
			m_iTicks++;
		}
		
		cout << m_sEndCharacter << flush;
	}

	void ProgressBar::Finish() { cout << endl; }
}

#endif // PROGBAR_CPP

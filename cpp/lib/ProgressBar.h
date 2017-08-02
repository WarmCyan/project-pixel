//*************************************************************
//  File: ProgressBar.h
//  Date created: 2/9/2017
//  Date edited: 7/27/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: Class for displaying task progress on a single line
//*************************************************************

#ifndef PROGBAR_H
#define PROGBAR_H

#include <iostream>
#include <string>
#include <cmath>

using namespace std;

namespace dwl
{
	class ProgressBar
	{
		private:
			//int m_iStep;
			//int m_iPrevStep;
			int m_iTicks;
			float m_fStepSize;

			int m_iSize;
			int m_iTotal;
			int m_iPrevTickCount;
			
			// █ = 219
			// ■ = 254
			
			//char m_cTickCharacter = '|';
			//char m_cEndCharacter = '.';
			//char m_cTickCharacter = 219; 
			string m_sEndCharacter = "|";
			string m_sTickCharacter = "\u2588";
			
		public:

			
			ProgressBar(int iTotal, int iSize);

			void Update(int iCount);
			void Finish();
	};
}

#endif // PROGBAR_H

//*************************************************************
//  File: FlameFractal.h
//  Date created: 1/28/2017
//  Date edited: 8/11/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************

#ifndef FF_H
#define FF_H

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "Function.h"
//#include "ProgressBar.h"
#include <ProgressBar.h>

//#include "lib/pugixml.hpp"
#include <pugixml.hpp>

#define PI 3.14159265

using namespace std;
using namespace pugi;

namespace dwl
{
	class FlameFractal
	{
		private:
			int m_iProgressBarSize = 80;

			bool m_bInitialized = false;

			bool m_bDivergent = false;
			
			int m_iWidth;
			int m_iHeight;

			int m_iIterations;

			float m_fTempR;
			float m_fTempG;
			float m_fTempB;

			float m_fTempX;
			float m_fTempY;

			// used only for solution start (TODO: break out the solve function
			// into initialization and actual solving, since solving can be
			// continued
			float m_fStartX;
			float m_fStartY;
			float m_fStartC;

			float m_fKernelScalar;
			float m_fKernelExpDenom;

			float m_fTraceX;
			float m_fTraceY;
			float m_fTraceC;

			// META
			float m_fOffsetX;
			float m_fOffsetY;

			float m_fZoomX;
			float m_fZoomY;

			float m_fScalarX; // calculated, not set
			float m_fScalarY; // calculated, not set
			float m_fRawOffsetX; // calculated, not set
			float m_fRawOffsetY; // calculated, not set

			// COLOR RAMPS
			vector<float>* m_vRampPoints = NULL;
			vector<vector<float> >* m_vRampVals = NULL;
			string m_sColorName = "";

			// image data stores
			vector<vector<vector<float> > >* m_vPoints = NULL;
			vector<vector<vector<float> > >* m_vImage = NULL;
			vector<vector<vector<float> > >* m_vPostProcImage = NULL;
			vector<vector<vector<int> > >* m_vFinalImage = NULL;
			
			// pdfs
			vector<float>* m_vPDFX = NULL;
			vector<float>* m_vPDFY = NULL;
			
			
			//vector<FFFunction> m_vFunctions = NULL;
			vector<FFFunction> m_vFunctions;


			void PlotPoint(float fX, float fY, float fC);

			void ColorMap(float fColor);
			void InitializeColorRamp(int iSize);

			void FinalTransform(float fX, float fY);
			float FinalColorTransform(float cX);

			void CopyImage(vector<vector<vector<float> > >* m_vInput, vector<vector<vector<float> > >* m_vOutput);

			// gaussian kernel blur stuff
			void CalculateKernelScalars(float fStdDev);
			float CalculateConvolutionForDistance(int iX, int iY);
			vector<vector<float> >* CalculateConvolutionMatrix(int iSize, float fStdDev, bool bDebug);

			void CalculatePointFactor(int iX, int iY, float fFactor, bool bDebug);
			void FilterPoint(int iX, int iY, vector<vector<float> >* vConvolutionMatrix, bool bDebug);

			float RandomFloat();


			
		public:

			FlameFractal(int iWidth, int iHeight);
			~FlameFractal();

			int GetWidth() { return m_iWidth; }
			int GetHeight() { return m_iHeight; }

			int GetIterations() { return m_iIterations; }

			void AddFunction(FFFunction pFunction) { m_vFunctions.push_back(pFunction); }
			void ClearFunctions() { m_vFunctions = vector<FFFunction>(); }
			vector<FFFunction> GetFunctions() { return m_vFunctions; }

			void SetOffsetX(float fOffsetX) { m_fOffsetX = fOffsetX; }
			void SetOffsetY(float fOffsetY) { m_fOffsetY = fOffsetY; }
			void SetOffsets(float fOffsetX, float fOffsetY) { m_fOffsetX = fOffsetX; m_fOffsetY = fOffsetY; }

			void SetZoomX(float fZoomX) { m_fZoomX = fZoomX; }
			void SetZoomY(float fZoomY) { m_fZoomY = fZoomY; }
			void SetZoom(float fZoomX, float fZoomY) { m_fZoomX = fZoomX; m_fZoomY = fZoomY; }
			float GetZoom() { return m_fZoomX; }

			void SetColorRamp(vector<float> vPoints, vector<vector<float> > vValues);
			void SetColorName(string sColorName) { m_sColorName = sColorName; }
			string GetColorName() { return m_sColorName; }

			bool IsInitialized() { return m_bInitialized; }
			
			void PreparePlot();
			void SetBaseImage(float fR, float fG, float fB, float fA);
			void InitializeSolution();
			void Solve(int iIterationCount);
			void Render(float fGamma, float fBrightness, int iFilterMethod, float fHistBlurWeight, float fDensityBlurWeight, float fSecondPassBlur);

			// TODO: don't forget, store functions as well!
			void SaveFunctionCode(string sFileName);
			void SaveImageTrace(string sFileName);

			void LoadImageTrace(string sFileName);
			void LoadFunctionCode(string sFileName);

			void SaveImageData(string sFileName);

			vector<vector<vector<int> > >* GetImage() { return m_vFinalImage; } // TODO: not sure if I need this anymore

			void SetProgressBarSize(int iSize) { m_iProgressBarSize = iSize; }
	};
}

#endif // FF_H

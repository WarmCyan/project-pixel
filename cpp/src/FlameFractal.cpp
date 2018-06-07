//*************************************************************
//  File: FlameFractal.cpp
//  Date created: 1/28/2017
//  Date edited: 06/07/2018
//  Author: Nathan Martindale
//  Copyright © 2018 Digital Warrior Labs
//  Description: 
//*************************************************************

#ifndef FF_CPP
#define FF_CPP

#include "FlameFractal.h"

using namespace std;

namespace dwl
{

	/*FlameFractal::FlameFractal()
	{
		FlameFractal(0,0);
	}*/
	
	FlameFractal::FlameFractal(int iWidth, int iHeight)
	{
		m_bInitialized = false;

		m_bDivergent = false;
		
		m_iWidth = iWidth;
		m_iHeight = iHeight;

		//m_vFunctions = new vector<FFFunction>();

		m_fTempR = 0.0f;
		m_fTempG = 0.0f;
		m_fTempB = 0.0f;

		m_fTempX = 0.0f;
		m_fTempY = 0.0f;

		m_fOffsetX = 0.0f;
		m_fOffsetY = 0.0f;
		
		m_fZoomX = 1.0f;
		m_fZoomY = 1.0f;
		
		m_fScalarX = 0.0f; 
		m_fScalarY = 0.0f; 
		m_fRawOffsetX = 0.0f; 
		m_fRawOffsetY = 0.0f;
	}
	FlameFractal::~FlameFractal()
	{
		delete m_vPoints;
		delete m_vImage;
		delete m_vPostProcImage;
		delete m_vFinalImage;
		delete m_vRampVals;
		delete m_vRampPoints;
		delete m_vPDFX;
		delete m_vPDFY;
		//m_vFunctions.clear();
	}

	float FlameFractal::RandomFloat()
	{
		random_device rd;
		srand(rd());
		float fRandom = ((float) rand()) / (float) RAND_MAX;

		return fRandom;
	}
	
	void FlameFractal::SetColorRamp(vector<float> vPoints, vector<vector<float> > vValues)
	{
		cout << "Setting color ramp..." << endl;
		InitializeColorRamp(vPoints.size());
		//m_vRampPoints = new vector<float>(vPoints.size(), 0.0f);
		//m_vRampVals = new vector<vector<float > >(vValues.size(), vector<float>(vValues[0].size(), 0.0f));

		for (int i = 0; i < m_vRampPoints->size(); i++)
		{
			(*m_vRampPoints)[i] = vPoints[i];
		}
		for (int i = 0; i < m_vRampVals->size(); i++)
		{
			for (int j = 0; j < (*m_vRampVals)[i].size(); j++)
			{
				(*m_vRampVals)[i][j] = vValues[i][j];
			}
		}

		cout << "Successfully set color ramp!" << endl;
	}

	void FlameFractal::InitializeColorRamp(int iSize)
	{
		m_vRampPoints = new vector<float>(iSize, 0.0f);
		m_vRampVals = new vector<vector<float > >(iSize, vector<float>(3, 0.0f));
	}
	

	void FlameFractal::PreparePlot()
	{
		cout << "Preparing plot..." << endl;

		ProgressBar pBar = ProgressBar(6, m_iProgressBarSize);

		m_vPoints = new vector<vector<vector<float> > >(m_iHeight, vector<vector<float> >(m_iWidth, vector<float>(4, 0)));
		pBar.Update(1);
		
		m_vImage = new vector<vector<vector<float> > >(m_iHeight, vector<vector<float> >(m_iWidth, vector<float>(4, 0)));
		pBar.Update(2);
		
		m_vPostProcImage = new vector<vector<vector<float> > >(m_iHeight, vector<vector<float> >(m_iWidth, vector<float>(4, 0)));
		pBar.Update(3);
		
		m_vFinalImage = new vector<vector<vector<int> > >(m_iHeight, vector<vector<int> >(m_iWidth, vector<int>(4, 0)));
		pBar.Update(4);

		m_vPDFX = new vector<float>(m_iWidth, 0);
		pBar.Update(5);
		
		m_vPDFY = new vector<float>(m_iHeight, 0);
		pBar.Update(6);
		
		pBar.Finish();
	
		cout << "Plot prepared!" << endl;
	}

	void FlameFractal::SetBaseImage(float fR, float fG, float fB, float fA)
	{
		cout << "Setting base color..." << endl;
		for (int y = 0; y < m_vFinalImage->size(); y++)
		{
			for (int x = 0; x < (*m_vFinalImage)[y].size(); x++)
			{
				float fDensity = (*m_vFinalImage)[y][x][3];
				float fR = (*m_vFinalImage)[y][x][0];
				float fG = (*m_vFinalImage)[y][x][1];
				float fB = (*m_vFinalImage)[y][x][2];
			}
		}
		cout << "Base color set!" << endl;
	}

	void FlameFractal::PlotPoint(float fX, float fY, float fC)
	{
		//cout << " p."; // DEBUG
		//cout << "Plotting point..." << endl; // DEBUG
		
		ColorMap(fC);

		int iCoordX = (int) fX;
		int iCoordY = (int) fY;

		//cout << "plotting: " << iCoordX << "," << iCoordY << endl; // DEBUG

		(*m_vPoints)[iCoordY][iCoordX][0] += m_fTempR;
		(*m_vPoints)[iCoordY][iCoordX][1] += m_fTempG;
		(*m_vPoints)[iCoordY][iCoordX][2] += m_fTempB;
		(*m_vPoints)[iCoordY][iCoordX][3] += 1.0f;

		// cout << "density: " << (*m_vPoints)[iCoordY][iCoordX][3] << endl; // DEBUG
		//cout << "!"; // DEBUG
		//cout << "Plotted point!" << endl; // DEBUG
	}

	void FlameFractal::ColorMap(float fColor)
	{
		// nice blue!
		//m_fTempR = 1.0f - fColor;
		//m_fTempG = 1.0f - (fColor / 2);
		//m_fTempB = 1.0f;
		//return;
		
		// very green!
		//m_fTempR = 1.0f - fColor;
		//m_fTempG = 1.0f;
		//m_fTempB = 1.0f - (fColor / 2);
	

		// get the ramp point AFTER the color
		int iIndex = 1; // don't start at 0 (otherwise index problems if the color is 0)
		while ((*m_vRampPoints)[iIndex] < fColor) { iIndex++; }

		float fRun = (*m_vRampPoints)[iIndex] - (*m_vRampPoints)[iIndex - 1];

		float fDist = fColor - (*m_vRampPoints)[iIndex-1];
		
		// find slope between points for each color
		float fSlopeR = ((*m_vRampVals)[iIndex][0] - (*m_vRampVals)[iIndex - 1][0]) / fRun;
		float fSlopeG = ((*m_vRampVals)[iIndex][1] - (*m_vRampVals)[iIndex - 1][1]) / fRun;
		float fSlopeB = ((*m_vRampVals)[iIndex][2] - (*m_vRampVals)[iIndex - 1][2]) / fRun;

		m_fTempR = (*m_vRampVals)[iIndex - 1][0] + fSlopeR*fDist;
		m_fTempG = (*m_vRampVals)[iIndex - 1][1] + fSlopeG*fDist;
		m_fTempB = (*m_vRampVals)[iIndex - 1][2] + fSlopeB*fDist;
	}

	void FlameFractal::FinalTransform(float fX, float fY)
	{
		/*m_fTempX = fX * 1000;
		m_fTempY = fY * 1000;*/
		
		//m_fTempX = fX * 640 + 1600;
		//m_fTempY = fY * 640 + 800;
		//m_fTempX = fX * 400 + 500;
		//m_fTempY = fY * 400 + 500;
		
		//m_fTempX = fX * 300 + 500;
		//m_fTempY = fY * 300 + 500;

		m_fTempX = fX * m_fScalarX + m_fRawOffsetX;
		m_fTempY = fY * m_fScalarY + m_fRawOffsetY;
	}

	float FlameFractal::FinalColorTransform(float fColor) { return fColor; }

	void FlameFractal::InitializeSolution()
	{
		cout << "Setting initial solution variables..." << endl;
			
		// choose a random starting point
		//float fX = RandomFloat() * 2 - 1;
		//float fY = RandomFloat() * 2 - 1;

		m_fStartX = RandomFloat() * 2 - 1;
		m_fStartY = RandomFloat() * 2 - 1;

		// choose a random starting color
		//float fC = RandomFloat();
		m_fStartC = RandomFloat();


		cout << "Calculating transform variables..." << endl;
		
		// determine smallest dimension
		float fBase = (m_iHeight < m_iWidth) ? (float)m_iHeight : (float)m_iWidth;
		m_fScalarX = fBase * m_fZoomX;
		m_fScalarY = fBase * m_fZoomY;
		m_fRawOffsetX = m_iWidth / 2 + m_fOffsetX;
		m_fRawOffsetY = m_iHeight / 2 + m_fOffsetY;
		cout << "Scalar X: " << m_fScalarX << endl;
		cout << "Scalar Y: " << m_fScalarY << endl;
		cout << "Offset X: " << m_fRawOffsetX << endl;
		cout << "Offset Y: " << m_fRawOffsetY << endl;
		//cout << "Base zoom scalar: " << fBase << endl; // DEBUG

		m_bInitialized = true;
	}

	//void FlameFractal::Solve(int iIterationCount)
	void FlameFractal::Solve(long int iIterationCount)
	{
		cout << "Solving..." << endl;
		m_iIterations = iIterationCount;

		// choose a random starting point
		/*float fX = RandomFloat() * 2 - 1;
		float fY = RandomFloat() * 2 - 1;
		float fX_f = fX;
		float fY_f = fY;

		// choose a random starting color
		float fC = RandomFloat();
		float fC_f = fC; */

		float fX = m_fStartX;
		float fY = m_fStartY;
		float fX_f = fX;
		float fY_f = fY;

		float fC = m_fStartC;
		float fC_f = fC;
		
		vector<int>* vFunctionCounts = new vector<int>(m_vFunctions.size(), 0);
			
		// determine the weighting of each function (out of 1 total)
		vector<float>* vFunctionWeights = new vector<float>(m_vFunctions.size(), 0.0f);
		float fFunctionWeightsTotal = 0.0f;


		// first pass just to get total weight
		for (int i = 0; i < m_vFunctions.size(); i++) { fFunctionWeightsTotal += m_vFunctions[i].GetWeight(); }

		// second pass to normalize the function weights 
		float fSumSoFar = 0.0f;
		for (int i = 0; i < m_vFunctions.size(); i++)
		{
			float fNormalizedWeight = m_vFunctions[i].GetWeight() / fFunctionWeightsTotal;
			fSumSoFar += fNormalizedWeight;
			(*vFunctionWeights)[i] = fSumSoFar;
		}

		// display function weight "cdf"
		cout << "Cumulative function weights: [";
		for (int i = 0; i < vFunctionWeights->size(); i++)
		{
			cout << (*vFunctionWeights)[i];
			if (i < vFunctionWeights->size() - 1) { cout << ", "; }
		}
		cout << "]" << endl;

		// run the chaos game!
		int iDivergetHoldSet = 10; // NOTE: set this to change number of steps before it starts plotting again
		int iDivergentHold = 0;
		ProgressBar pBar = ProgressBar((iIterationCount / 1000), m_iProgressBarSize); // dividing by 1000 so progress bar doesn't freak out with large int
		for (long int iIteration = 0; iIteration <= iIterationCount; iIteration++)
		{
			//cout << "Iteration: " << iIteration << endl; // DEBUG

			//cout << "\n" << iIteration;  // DEBUG
			// choose a random function
			float fDice = RandomFloat();
			int iSelectedFunction = 0;
			//cout << "Dice: " << fDice << endl; // DEBUG
			while (fDice > (*vFunctionWeights)[iSelectedFunction]) { iSelectedFunction++; }
			if (iSelectedFunction >= m_vFunctions.size()) { iSelectedFunction = m_vFunctions.size() - 1; } // otherwise I think it sometimes gets floating point errors and ends up way beyond the num of functions (when the dice roll is exactly 1) and this causes segmentation fault
			(*vFunctionCounts)[iSelectedFunction]++;
			
			//cout << "Selected function " << iSelectedFunction << endl; // DEBUG

			//cout << " 1"; // DEBUG

			// run the function
			m_vFunctions[iSelectedFunction].Run(fX, fY);
			fX = m_vFunctions[iSelectedFunction].GetResultX();
			fY = m_vFunctions[iSelectedFunction].GetResultY();
			
			//cout << "Ran function " << fX << " " << fY << endl; // DEBUG

			//cout << " 2"; // DEBUG
			//cout << " " << fX << "," << fY; // DEBUG

			// check for divergent solutions
			//int iDivergentLimit = 10000000000; // NOTE: was previously 10000000000
			if (fY < -10000000000 || fY > 10000000000 || fY < -10000000000 || fY > 10000000000)
			{
				//pBar.Finish();
				cout << "WARNING - Solution diverges, skipping further iterations..." << endl;
				fX = 0.0f;
				fY = 0.0f;
				m_bDivergent = true;
				iDivergentHold = iDivergetHoldSet;
				//break;
			}

			// run final transform
			FinalTransform(fX, fY);
			fX_f = m_fTempX;
			fY_f = m_fTempY;

			//cout << "Final Transform " << fX_f << " " << fY_f << endl; // DEBUG
			//cout << " 3"; // DEBUG
			//cout << "Final transform worked" << endl; //DEBUG

			//cout << " " << fX_f << "," << fY_f; // DEBUG

			// run color stuff if the running function isn't a symmetry function
			if (!m_vFunctions[iSelectedFunction].IsSymmetry())
			{
				//cout << "Running symmetry" << endl; // DEBUG
				fC = (fC + m_vFunctions[iSelectedFunction].GetColor()) / 2.0f;
				fC_f = (fC + FinalColorTransform(fC)) / 2.0f;
				//cout << "Ran symmetry" << endl; // DEBUG
			}
			//cout << " 4"; // DEBUG

			// store trace information
			m_fTraceX = fX;
			m_fTraceY = fY;
			m_fTraceC = fC;

			// ignore the first 20 iterations, (to allow convergence below size
			// of pixel) then plot each point
			if (iIteration > 20 && (!m_bDivergent || iDivergentHold > 0))
			{
				if (fX_f < 0 || fX_f >= m_iWidth || fY_f < 0 || fY_f >= m_iHeight) { continue; }
				PlotPoint(fX_f, fY_f, fC_f);
			}
			if (iDivergentHold > 0)
			{
				iDivergentHold--;
				//iIteration--; // NOTE: This may not be a good idea...
			}
			if (m_bDivergent) { m_bDivergent = false; }
			//cout << " 5"; // DEBUG

			pBar.Update((iIteration / 1000));
		}
		pBar.Finish();

		if (!m_bDivergent) { cout << "Solving complete!" << endl; }
	}

	void FlameFractal::CopyImage(vector<vector<vector<float> > >* m_vInput, vector<vector<vector<float> > >* m_vOutput)
	{
		for (int y = 0; y < m_vInput->size(); y++)
		{
			for (int x = 0; x < (*m_vInput)[y].size(); x++)
			{
				(*m_vOutput)[y][x] = (*m_vInput)[y][x];
			}
		}
	}

	void FlameFractal::CalculateKernelScalars(float fStdDev)
	{
		m_fKernelScalar = 1 / (2 * PI * pow(fStdDev, 2));
		m_fKernelExpDenom = 2 * pow(fStdDev, 2);
	}
	float FlameFractal::CalculateConvolutionForDistance(int iX, int iY)
	{
		return m_fKernelScalar * exp(-((pow(iX, 2) + pow(iY, 2)) / m_fKernelExpDenom));
	}
	vector<vector<float> >* FlameFractal::CalculateConvolutionMatrix(int iSize, float fStdDev, bool bDebug)
	{
		CalculateKernelScalars(fStdDev);

		int iMatrixSize = iSize * 2 + 1;
		float fMatrixSum = 0.0f; // used for normalization
		vector<vector<float> >* vMatrix = new vector<vector<float > >(iMatrixSize, vector<float>(iMatrixSize, 0));

		if (bDebug) { cout << "iSize: " << iSize << " MatrixSize: " << iMatrixSize << endl; }

		//for (int y = -iSize; y < iSize + 1; y++) // TODO: shouldn't that be iSize + 1?
		for (int y = 0; y < iMatrixSize; y++)
		{
			int iY = y - iSize;
			for (int x = 0; x < iMatrixSize; x++)
			{
				int iX = x - iSize;

				float fConvolutionValue = CalculateConvolutionForDistance(iX, iY);
				(*vMatrix)[y][x] = fConvolutionValue;
				fMatrixSum += fConvolutionValue;
			}
		}

		// normalize
		for (int y = 0; y < iMatrixSize; y++)
		{
			for (int x = 0; x < iMatrixSize; x++)
			{
				(*vMatrix)[y][x] /= fMatrixSum;
				if (bDebug) { cout << "matrix value: " << (*vMatrix)[y][x] << endl; }
			}
		}

		return vMatrix;
	}
	
	void FlameFractal::CalculatePointFactor(int iX, int iY, float fFactor, bool bDebug)
	{
		m_fTempR = 0.0f;
		m_fTempG = 0.0f;
		m_fTempB = 0.0f;

		if (bDebug) { cout << "on coord " << iX << "," << iY << endl; }

		if (iX < m_iWidth && iX >= 0 && iY < m_iHeight && iY >= 0)
		{
			m_fTempR = (*m_vImage)[iY][iX][0] * fFactor;
			m_fTempG = (*m_vImage)[iY][iX][1] * fFactor;
			m_fTempB = (*m_vImage)[iY][iX][2] * fFactor;

			if (bDebug) { cout << (*m_vPoints)[iY][iX][3] << endl; }
		}
	}
	void FlameFractal::FilterPoint(int iX, int iY, vector<vector<float> >* vConvolutionMatrix, bool bDebug)
	{
		float fR = 0.0f;
		float fG = 0.0f;
		float fB = 0.0f;

		for (int ly = 0; ly < vConvolutionMatrix->size(); ly++)
		{
			int iYOffset = ly - floor(vConvolutionMatrix->size() / 2);
			if (bDebug) { cout << "on y offset " << iYOffset << endl; }
			for (int lx = 0; lx < (*vConvolutionMatrix)[ly].size(); lx++)
			{
				int iXOffset = lx - floor((*vConvolutionMatrix)[ly].size() / 2);
				if (bDebug) { cout << "\ton x offset " << iXOffset << endl; }
				if (bDebug) { cout << "\t\t" << (*vConvolutionMatrix)[ly][lx] << endl; }
				CalculatePointFactor(iX + iXOffset, iY + iYOffset, (*vConvolutionMatrix)[ly][lx], bDebug);

				if (bDebug) { cout << "\t\t" << m_fTempR << "," << m_fTempG << "," << m_fTempB << endl; }

				fR += m_fTempR;
				fG += m_fTempG;
				fB += m_fTempB;
			}
		}
		if (bDebug) { cout << fR << "," << fG << "," << fB << endl; }

		m_fTempR = fR;
		m_fTempG = fG;
		m_fTempB = fB;
	}

	void FlameFractal::Render(float fGamma, float fBrightness, int iFilterMethod, float fHistBlurWeight, float fDensityBlurWeight, float fSecondPassBlur)
	{
		cout << "Rendering... (gamma = " << fGamma << ", brightness = " << fBrightness << ")" << endl;

		// FIRST PASS
		cout << "First pass... (gathering data)" << endl;
		int iDensityCounts = 0;
		float fTotalDensity = 0.0f;
		float fMaxDensity = 0.0f;

		ProgressBar pBar1 = ProgressBar(m_vPoints->size() - 1, m_iProgressBarSize);

		for (int y = 0; y < m_vPoints->size(); y++)
		{
			pBar1.Update(y);
			for (int x = 0; x < (*m_vPoints)[y].size(); x++)
			{
				float fDensity = (*m_vPoints)[y][x][3];
				if (fDensity > 1) // NOTE: this is technically an error (should be > 0), but I prefer the brightness scalar that results from this
				{
					if (fDensity > fMaxDensity) { fMaxDensity = fDensity; }
					fTotalDensity += fDensity;
					iDensityCounts++;
				}
			}
		}
		pBar1.Finish();

		float fAverageDensity = fTotalDensity / (float) iDensityCounts;
		cout << "Average density: " << fAverageDensity << endl;
		cout << "Highest density: " << fMaxDensity << endl;

		float fBrightnessScalar = (255 / fAverageDensity) * fBrightness;
		cout << "Brightness scalar: " << fBrightnessScalar << endl;

		
		// SECOND PASS
		cout << "Second pass... (Resolving colors, applying gamma and brightness corrections)" << endl;


		//CopyImage(m_vPoints, m_vImage);
		for (int y = 0; y < m_vPoints->size(); y++)
		{
			for (int x = 0; x < (*m_vPoints)[y].size(); x++)
			{
				(*m_vImage)[y][x][0] = (*m_vPoints)[y][x][0];
				(*m_vImage)[y][x][1] = (*m_vPoints)[y][x][1];
				(*m_vImage)[y][x][2] = (*m_vPoints)[y][x][2];
				(*m_vImage)[y][x][3] = (*m_vPoints)[y][x][3];
			}
		}			

		// determine axis histograms
		float fActualTotalDensity = 0.0f;
		float fMaxXHist = 0.0f;
		float fMaxYHist = 0.0f;
		for (int y = 0; y < m_iHeight; y++)
		{
			float fRowDensity = 0.0f;
			for (int x = 0; x < (*m_vImage)[y].size(); x++) { fRowDensity += (*m_vImage)[y][x][3]; }

			(*m_vPDFY)[y] = fRowDensity;
			fActualTotalDensity += fRowDensity;
		}
		for (int x = 0; x < m_iWidth; x++)
		{
			float fColDensity = 0.0f;
			for (int y = 0; y < m_iHeight; y++) { fColDensity += (*m_vImage)[y][x][3]; }

			(*m_vPDFX)[x] = fColDensity;
		}
		for (int i = 0; i < m_vPDFX->size(); i++) 
		{ 
			(*m_vPDFX)[i] = (*m_vPDFX)[i] / fActualTotalDensity; 
			//cout << (*m_vPDFX)[i] << endl;
			if ((*m_vPDFX)[i] > fMaxXHist) { fMaxXHist = (*m_vPDFX)[i]; }
		}
		for (int i = 0; i < m_vPDFY->size(); i++) 
		{ 
			(*m_vPDFY)[i] = (*m_vPDFY)[i] / fActualTotalDensity; 
			if ((*m_vPDFY)[i] > fMaxYHist) { fMaxYHist = (*m_vPDFY)[i]; }
		}
		
		ProgressBar pBar2 = ProgressBar(m_vImage->size() - 1, m_iProgressBarSize);

		for (int y = 0; y < m_vImage->size(); y++)
		{
			pBar2.Update(y);
			for (int x = 0; x < (*m_vImage)[y].size(); x++)
			{
				float fDensity = (*m_vImage)[y][x][3];
				float fR = (*m_vImage)[y][x][0];
				float fG = (*m_vImage)[y][x][1];
				float fB = (*m_vImage)[y][x][2];

				if (fDensity > 1) // NOTE: If this is > 0, get weird white spots. DON'T TOUCH!
				{
					// log-density brightness scalar
					float fLogScalar = log(fDensity) / fDensity;

					// gamma correction
					float fLogScalar_c = pow(fLogScalar, 1 / fGamma);

					// resolve rgb colors and apply brightness scalars
					fR *= fLogScalar_c * fBrightnessScalar;
					fG *= fLogScalar_c * fBrightnessScalar;
					fB *= fLogScalar_c * fBrightnessScalar;

					// cap overflows
					fR = min(255.0f, fR);
					fG = min(255.0f, fG);
					fB = min(255.0f, fB);

					// reassign
					(*m_vImage)[y][x] = { fR, fG, fB, 255.0f };

				}
				(*m_vImage)[y][x][3] = 255.0f;
				//cout << fR << fG << fB << endl;
				//cout << fDensity << endl;
			}
		}
		pBar2.Finish();


		// THIRD PASS
		cout << "Third pass... (Prestage noise removal)" << endl;
		
		for (int y = 0; y < m_vPostProcImage->size(); y++)
		{
			for (int x = 0; x < (*m_vPostProcImage)[y].size(); x++)
			{
				(*m_vPostProcImage)[y][x][0] = (*m_vImage)[y][x][0];
				(*m_vPostProcImage)[y][x][1] = (*m_vImage)[y][x][1];
				(*m_vPostProcImage)[y][x][2] = (*m_vImage)[y][x][2];
				(*m_vPostProcImage)[y][x][3] = (*m_vImage)[y][x][3];
			}
		}			

		// check for lone speckles
		for (int i = 0; i < 1; i++)
		{

			for (int y = 0; y < m_vPostProcImage->size(); y++)
			{
				for (int x = 0; x < (*m_vPostProcImage)[y].size(); x++)
				{
					(*m_vImage)[y][x][0] = (*m_vPostProcImage)[y][x][0];
					(*m_vImage)[y][x][1] = (*m_vPostProcImage)[y][x][1];
					(*m_vImage)[y][x][2] = (*m_vPostProcImage)[y][x][2];
					(*m_vImage)[y][x][3] = (*m_vPostProcImage)[y][x][3];
				}
			}			
			
			ProgressBar pBar3 = ProgressBar(m_vImage->size() - 1, m_iProgressBarSize);
			for (int y = 1; y < m_vPostProcImage->size() - 1; y++)
			{
				pBar3.Update(y);
				//cout << "row " << y << endl;
				for (int x = 1; x < (*m_vPostProcImage)[y].size() - 1; x++)
				{
					float fR = (*m_vImage)[y][x][0];
					float fG = (*m_vImage)[y][x][1];
					float fB = (*m_vImage)[y][x][2];
					//float fDensity = (*m_vPoints)[y][x][3];
					//float n = fDensity;
					//if (n < 1) { n = 1; }

					float fSum = fR + fG + fB;
					float fRatio = (fSum * 3) / 4;
					//bool noise = true;
					int noise = 2;

					float fRAvg = 0.0f;
					float fBAvg = 0.0f;
					float fGAvg = 0.0f;

					// upper left
					float fR1 = (*m_vImage)[y-1][x-1][0];
					float fG1 = (*m_vImage)[y-1][x-1][1];
					float fB1 = (*m_vImage)[y-1][x-1][2];

					float fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// upper middle
					fR1 = (*m_vImage)[y-1][x][0];
					fG1 = (*m_vImage)[y-1][x][1];
					fB1 = (*m_vImage)[y-1][x][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// upper right
					fR1 = (*m_vImage)[y-1][x+1][0];
					fG1 = (*m_vImage)[y-1][x+1][1];
					fB1 = (*m_vImage)[y-1][x+1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// middle left
					fR1 = (*m_vImage)[y][x-1][0];
					fG1 = (*m_vImage)[y][x-1][1];
					fB1 = (*m_vImage)[y][x-1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// middle right
					fR1 = (*m_vImage)[y][x+1][0];
					fG1 = (*m_vImage)[y][x+1][1];
					fB1 = (*m_vImage)[y][x+1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// lower left
					fR1 = (*m_vImage)[y+1][x-1][0];
					fG1 = (*m_vImage)[y+1][x-1][1];
					fB1 = (*m_vImage)[y+1][x-1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// lower middle
					fR1 = (*m_vImage)[y+1][x][0];
					fG1 = (*m_vImage)[y+1][x][1];
					fB1 = (*m_vImage)[y+1][x][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// lower right
					fR1 = (*m_vImage)[y+1][x+1][0];
					fG1 = (*m_vImage)[y+1][x+1][1];
					fB1 = (*m_vImage)[y+1][x+1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }

					if (noise < 0) { continue; } // TODO: should be <=

					fRAvg /= 8;
					fGAvg /= 8;
					fBAvg /= 8;

					if (noise == 2)
					{
						(*m_vPostProcImage)[y][x][0] = fR*.20 + fRAvg*.80;
						(*m_vPostProcImage)[y][x][1] = fG*.20 + fGAvg*.80;
						(*m_vPostProcImage)[y][x][2] = fB*.20 + fBAvg*.80;
					}
					else if (noise == 1)
					{
						(*m_vPostProcImage)[y][x][0] = fR*.4 + fRAvg*.6;
						(*m_vPostProcImage)[y][x][1] = fG*.4 + fGAvg*.6;
						(*m_vPostProcImage)[y][x][2] = fB*.4 + fBAvg*.6;
					}
					else if (noise == 0)
					{
						(*m_vPostProcImage)[y][x][0] = fR*.7 + fRAvg*.3;
						(*m_vPostProcImage)[y][x][1] = fG*.7 + fGAvg*.3;
						(*m_vPostProcImage)[y][x][2] = fB*.7 + fBAvg*.3;
					}
				}
				
			}
			pBar3.Finish();
		}

		

		// FOURTH PASS
		
		cout << "Fourth pass... (Post processing/filtering)" << endl;

		//if (m_bDivergent) { cout << "WARNING - Divergent solution, skipping all post proc..." << endl; iFilterMethod = 0; }

		//m_vPostProcImage = m_vImage;
		/*for (int y = 0; y < m_vImage->size(); y++)
		{
			for (int x = 0; x < (*m_vImage)[y].size(); x++)
			{
				(*m_vPostProcImage)[y][x][0] = (*m_vImage)[y][x][0];
				(*m_vPostProcImage)[y][x][1] = (*m_vImage)[y][x][1];
				(*m_vPostProcImage)[y][x][2] = (*m_vImage)[y][x][2];
				(*m_vPostProcImage)[y][x][3] = (*m_vImage)[y][x][3];
			}
		}			*/

		
		if (iFilterMethod > 0)
		{
			//smooth pdfs
			for (int x = 0; x < m_vPDFX->size() - 8; x+=8)
			{
				float fPoint1 = (*m_vPDFX)[x];
				float fPoint2 = (*m_vPDFX)[x+8];
				float fSlope = (fPoint2 - fPoint1) / 8;
				
				(*m_vPDFX)[x+1] = fPoint1 + fSlope;
				(*m_vPDFX)[x+2] = fPoint1 + fSlope*2;
				(*m_vPDFX)[x+3] = fPoint1 + fSlope*3;
				(*m_vPDFX)[x+4] = fPoint1 + fSlope*4;
				(*m_vPDFX)[x+5] = fPoint1 + fSlope*5;
				(*m_vPDFX)[x+6] = fPoint1 + fSlope*6;
				(*m_vPDFX)[x+7] = fPoint1 + fSlope*7;
				(*m_vPDFX)[x+8] = fPoint1 + fSlope*8;
			}
			for (int y = 0; y < m_vPDFY->size() - 8; y+=8)
			{
				float fPoint1 = (*m_vPDFY)[y];
				float fPoint2 = (*m_vPDFY)[y+8];
				float fSlope = (fPoint2 - fPoint1) / 8;
				
				(*m_vPDFY)[y+1] = fPoint1 + fSlope;
				(*m_vPDFY)[y+2] = fPoint1 + fSlope*2;
				(*m_vPDFY)[y+3] = fPoint1 + fSlope*3;
				(*m_vPDFY)[y+4] = fPoint1 + fSlope*4;
				(*m_vPDFY)[y+5] = fPoint1 + fSlope*5;
				(*m_vPDFY)[y+6] = fPoint1 + fSlope*6;
				(*m_vPDFY)[y+7] = fPoint1 + fSlope*7;
				(*m_vPDFY)[y+8] = fPoint1 + fSlope*8;
			}

			
			ProgressBar pBar4 = ProgressBar(m_vPostProcImage->size() - 1, m_iProgressBarSize);
			for (int y = 0; y < m_vPostProcImage->size(); y++)
			{
				pBar4.Update(y);
				//cout << "row " << y << endl;
				for (int x = 0; x < (*m_vPostProcImage)[y].size(); x++)
				{
					float fR = (*m_vImage)[y][x][0];
					float fG = (*m_vImage)[y][x][1];
					float fB = (*m_vImage)[y][x][2];
					float fDensity = (*m_vPoints)[y][x][3];

					float n = fDensity;
					if (n < 1) { n = 1; }

					
					float fStdDev = 0.0f;
					/*if (iFilterMethod == 1) { fStdDev = 5 * (1 / n); }
					else if (iFilterMethod == 2) { float fStdDev = min(5.0f, fAverageDensity / n); }*/
					
					float fYHist = (*m_vPDFY)[y];
					float fXHist = (*m_vPDFX)[x];

					float fYHistRatio = fYHist / fMaxYHist;
					float fXHistRatio = fXHist / fMaxXHist;

					//cout << fYHistRatio << " " << fXHistRatio << endl;

					//fStdDev = 5 / (fYHistRatio + fXHistRatio) * 1/(2*n/fAverageDensity); // dis good
					
					//fStdDev = 4 / (fYHistRatio + fXHistRatio) * 1/(2*n/fAverageDensity); // dis good (good defaults)
					fStdDev = (fHistBlurWeight * (4 / (fYHistRatio + fXHistRatio))) * (fDensityBlurWeight * (1/(2*n/fAverageDensity))); // dis good

					//fStdDev = 1 / (fYHist + fXHist) / 500;
					//fStdDev *= fStdDev;
					//fStdDev *= fStdDev;
					
					//fStdDev = 1.0f;
					/*if (n < (fAverageDensity / 5)) { fStdDev = 3 * (1/(n/fAverageDensity)); }
					else if (n < (fAverageDensity / 4)) { fStdDev = 2 * (1/(n/fAverageDensity)); }
					else if (n < (fAverageDensity / 3)) { fStdDev = 1 * (1/(n/fAverageDensity)); }
					else if (n < (fAverageDensity / 2)) { fStdDev = .5 * (1/(n/fAverageDensity)); }
					else { fStdDev = .25 * (1/(n/fAverageDensity)); }*/
					//fStdDev = (1 / (n/fAverageDensity));
					//fStdDev *= fStdDev;
					//fStdDev = max(30*(-(1/(2*(fMaxDensity/4)))*n+1), 0.0f);
					//fStdDev = max(20*(-(1/(2*(fAverageDensity/4)))*n+1), 0.0f);
					//else if (n < (fAverageDensity / 2)) { fStdDev = (1/(n/fAverageDensity)); }
					//else { fStdDev = .2; }
					//cout << fStdDev << endl;
					
					//fStdDev = 5; // DEBUG
					//int iSize = max(min((int)fStdDev * 3, 30), 1); // has to be at least 1!
					int iSize = max(min((int)fStdDev * 3, 60), 1); // has to be at least 1!

					

					//cout << fStdDev << endl; // DEBUG
					if (fStdDev > .1) // comment only for debug
					{
						vector<vector<float> >* vMatrix = CalculateConvolutionMatrix(iSize, fStdDev, false);
						FilterPoint(x, y, vMatrix, false);
						fR = m_fTempR;
						fG = m_fTempG;
						fB = m_fTempB;

						delete vMatrix;
					}


					// 60-.2
					//fR = max(.033333f * fStdDev - 1, 0);
					/*cout << fStdDev << endl;
					fStdDev = min(fStdDev, 30.0f);
					fR = 255*(min((fStdDev / 15.0f), 1.0f));
					fG = 255*(min(-(fStdDev / 15.0f) + 2, 1.0f));
					fB = 0.0f;*/
					
					// display histograms
					//if (x == x)
					//{
						//fR = fG = fB = (*m_vPDFY)[y]*25500;
						//if (x == 0) { cout << fR << endl; }
					//}

					//cout << fR << " " << fG << endl;

					(*m_vPostProcImage)[y][x][0] = fR;
					(*m_vPostProcImage)[y][x][1] = fG;
					(*m_vPostProcImage)[y][x][2] = fB;

					/*if (fB == 0 && (*m_vImage)[y][x][2] != 0)
					{
						cout << "actual blue for " << x << "," << y << ": " << (*m_vImage)[y][x][2] << endl;
						cout << "WARNING - zeroed" << endl;
						vector<vector<float> >* vMatrix = CalculateConvolutionMatrix(iSize, fStdDev, true);
						FilterPoint(x, y, vMatrix, true);
						return;
					}*/
				}
			}
			pBar4.Finish();
		}
		

		// FIFTH PASS
		cout << "Fifth pass... (second post-proc)" << endl;	
		
		if (iFilterMethod > 0)
		{
			for (int y = 0; y < m_vPostProcImage->size(); y++)
			{
				for (int x = 0; x < (*m_vPostProcImage)[y].size(); x++)
				{
					(*m_vImage)[y][x][0] = (*m_vPostProcImage)[y][x][0];
					(*m_vImage)[y][x][1] = (*m_vPostProcImage)[y][x][1];
					(*m_vImage)[y][x][2] = (*m_vPostProcImage)[y][x][2];
					(*m_vImage)[y][x][3] = (*m_vPostProcImage)[y][x][3];
				}
			}			
			
			ProgressBar pBar5 = ProgressBar(m_vPostProcImage->size() - 1, m_iProgressBarSize);
			for (int y = 0; y < m_vPostProcImage->size(); y++)
			{
				pBar5.Update(y);
				//cout << "row " << y << endl;
				for (int x = 0; x < (*m_vPostProcImage)[y].size(); x++)
				{
					float fR = (*m_vImage)[y][x][0];
					float fG = (*m_vImage)[y][x][1];
					float fB = (*m_vImage)[y][x][2];
					//float fDensity = (*m_vPoints)[y][x][3];

					//float n = fDensity;
					//if (n < 1) { n = 1; }

					
					//float fStdDev = 0.2f; // dis good default
					float fStdDev = fSecondPassBlur;



					
					/*if (iFilterMethod == 1) { fStdDev = 5 * (1 / n); }
					else if (iFilterMethod == 2) { float fStdDev = min(5.0f, fAverageDensity / n); }*/
					
					//float fYHist = (*m_vPDFY)[y];
					//float fXHist = (*m_vPDFX)[x];

					//float fYHistRatio = fYHist / fMaxYHist;
					//float fXHistRatio = fXHist / fMaxXHist;

					//cout << fYHistRatio << " " << fXHistRatio << endl;

					//fStdDev = 5 / (fYHistRatio + fXHistRatio) * 1/(2*n/fAverageDensity);

					//fStdDev = 1 / (fYHist + fXHist) / 500;
					//fStdDev *= fStdDev;
					//fStdDev *= fStdDev;
					
					//fStdDev = 1.0f;
					/*if (n < (fAverageDensity / 5)) { fStdDev = 3 * (1/(n/fAverageDensity)); }
					else if (n < (fAverageDensity / 4)) { fStdDev = 2 * (1/(n/fAverageDensity)); }
					else if (n < (fAverageDensity / 3)) { fStdDev = 1 * (1/(n/fAverageDensity)); }
					else if (n < (fAverageDensity / 2)) { fStdDev = .5 * (1/(n/fAverageDensity)); }
					else { fStdDev = .25 * (1/(n/fAverageDensity)); }*/
					//fStdDev = (1 / (n/fAverageDensity));
					//fStdDev *= fStdDev;
					//fStdDev = max(30*(-(1/(2*(fMaxDensity/4)))*n+1), 0.0f);
					//fStdDev = max(20*(-(1/(2*(fAverageDensity/4)))*n+1), 0.0f);
					//else if (n < (fAverageDensity / 2)) { fStdDev = (1/(n/fAverageDensity)); }
					//else { fStdDev = .2; }
					//cout << fStdDev << endl;
					
					//fStdDev = 5; // DEBUG
					//int iSize = max(min((int)fStdDev * 3, 30), 1); // has to be at least 1!
					int iSize = max(min((int)fStdDev * 3, 60), 1); // has to be at least 1!

					

					//cout << fStdDev << endl; // DEBUG
					if (fStdDev > .1) // comment only for debug
					{
						vector<vector<float> >* vMatrix = CalculateConvolutionMatrix(iSize, fStdDev, false);
						FilterPoint(x, y, vMatrix, false);
						fR = m_fTempR;
						fG = m_fTempG;
						fB = m_fTempB;

						delete vMatrix;
					}


					// 60-.2
					//fR = max(.033333f * fStdDev - 1, 0);
					/*cout << fStdDev << endl;
					fStdDev = min(fStdDev, 30.0f);
					fR = 255*(min((fStdDev / 15.0f), 1.0f));
					fG = 255*(min(-(fStdDev / 15.0f) + 2, 1.0f));
					fB = 0.0f;*/
					
					// display histograms
					//if (x == x)
					//{
						//fR = fG = fB = (*m_vPDFY)[y]*25500;
						//if (x == 0) { cout << fR << endl; }
					//}

					//cout << fR << " " << fG << endl;

					(*m_vPostProcImage)[y][x][0] = fR;
					(*m_vPostProcImage)[y][x][1] = fG;
					(*m_vPostProcImage)[y][x][2] = fB;

					/*if (fB == 0 && (*m_vImage)[y][x][2] != 0)
					{
						cout << "actual blue for " << x << "," << y << ": " << (*m_vImage)[y][x][2] << endl;
						cout << "WARNING - zeroed" << endl;
						vector<vector<float> >* vMatrix = CalculateConvolutionMatrix(iSize, fStdDev, true);
						FilterPoint(x, y, vMatrix, true);
						return;
					}*/
				}
			}
			pBar5.Finish();
		}


		
		// SIXTH PASS
		cout << "Sixth pass... (Post noise removal)" << endl;

		// check for lone speckles
		for (int i = 0; i < 0; i++)
		{

			for (int y = 0; y < m_vPostProcImage->size(); y++)
			{
				for (int x = 0; x < (*m_vPostProcImage)[y].size(); x++)
				{
					(*m_vImage)[y][x][0] = (*m_vPostProcImage)[y][x][0];
					(*m_vImage)[y][x][1] = (*m_vPostProcImage)[y][x][1];
					(*m_vImage)[y][x][2] = (*m_vPostProcImage)[y][x][2];
					(*m_vImage)[y][x][3] = (*m_vPostProcImage)[y][x][3];
				}
			}			
			
			ProgressBar pBar6 = ProgressBar(m_vImage->size() - 1, m_iProgressBarSize);
			for (int y = 1; y < m_vPostProcImage->size() - 1; y++)
			{
				pBar6.Update(y);
				//cout << "row " << y << endl;
				for (int x = 1; x < (*m_vPostProcImage)[y].size() - 1; x++)
				{
					float fR = (*m_vImage)[y][x][0];
					float fG = (*m_vImage)[y][x][1];
					float fB = (*m_vImage)[y][x][2];
					//float fDensity = (*m_vPoints)[y][x][3];
					//float n = fDensity;
					//if (n < 1) { n = 1; }

					float fSum = fR + fG + fB;
					float fRatio = (fSum * 3) / 4;
					//bool noise = true;
					int noise = 2;

					float fRAvg = 0.0f;
					float fBAvg = 0.0f;
					float fGAvg = 0.0f;

					// upper left
					float fR1 = (*m_vImage)[y-1][x-1][0];
					float fG1 = (*m_vImage)[y-1][x-1][1];
					float fB1 = (*m_vImage)[y-1][x-1][2];

					float fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// upper middle
					fR1 = (*m_vImage)[y-1][x][0];
					fG1 = (*m_vImage)[y-1][x][1];
					fB1 = (*m_vImage)[y-1][x][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// upper right
					fR1 = (*m_vImage)[y-1][x+1][0];
					fG1 = (*m_vImage)[y-1][x+1][1];
					fB1 = (*m_vImage)[y-1][x+1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// middle left
					fR1 = (*m_vImage)[y][x-1][0];
					fG1 = (*m_vImage)[y][x-1][1];
					fB1 = (*m_vImage)[y][x-1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// middle right
					fR1 = (*m_vImage)[y][x+1][0];
					fG1 = (*m_vImage)[y][x+1][1];
					fB1 = (*m_vImage)[y][x+1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// lower left
					fR1 = (*m_vImage)[y+1][x-1][0];
					fG1 = (*m_vImage)[y+1][x-1][1];
					fB1 = (*m_vImage)[y+1][x-1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// lower middle
					fR1 = (*m_vImage)[y+1][x][0];
					fG1 = (*m_vImage)[y+1][x][1];
					fB1 = (*m_vImage)[y+1][x][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }
					
					// lower right
					fR1 = (*m_vImage)[y+1][x+1][0];
					fG1 = (*m_vImage)[y+1][x+1][1];
					fB1 = (*m_vImage)[y+1][x+1][2];

					fSum1 = fR1 + fG1 + fB1;
					fRAvg += fR1;
					fGAvg += fG1;
					fBAvg += fB1;

					if (fSum1 > (fRatio)) { noise--; }

					if (noise < 0) { continue; } // TODO: should be <=

					fRAvg /= 8;
					fGAvg /= 8;
					fBAvg /= 8;

					if (noise == 2)
					{
						(*m_vPostProcImage)[y][x][0] = fR*.20 + fRAvg*.80;
						(*m_vPostProcImage)[y][x][1] = fG*.20 + fGAvg*.80;
						(*m_vPostProcImage)[y][x][2] = fB*.20 + fBAvg*.80;
					}
					else if (noise == 1)
					{
						(*m_vPostProcImage)[y][x][0] = fR*.4 + fRAvg*.6;
						(*m_vPostProcImage)[y][x][1] = fG*.4 + fGAvg*.6;
						(*m_vPostProcImage)[y][x][2] = fB*.4 + fBAvg*.6;
					}
					else if (noise == 0)
					{
						(*m_vPostProcImage)[y][x][0] = fR*.7 + fRAvg*.3;
						(*m_vPostProcImage)[y][x][1] = fG*.7 + fGAvg*.3;
						(*m_vPostProcImage)[y][x][2] = fB*.7 + fBAvg*.3;
					}
				}
				
			}
			pBar6.Finish();
		}

		

		// SEVENTH PASS
		cout << "Seventh pass... (final transformations)" << endl;

		ProgressBar pBar7 = ProgressBar(m_vImage->size() - 1, m_iProgressBarSize);

		for (int y = 0; y < m_vImage->size(); y++)
		{
			pBar7.Update(y);
			for (int x = 0; x < (*m_vImage)[y].size(); x++)
			{
				/*(*m_vFinalImage)[y][x][0] = (int)((*m_vImage)[y][x][0]);
				(*m_vFinalImage)[y][x][1] = (int)((*m_vImage)[y][x][1]);
				(*m_vFinalImage)[y][x][2] = (int)((*m_vImage)[y][x][2]);
				(*m_vFinalImage)[y][x][3] = (int)((*m_vImage)[y][x][3]);*/
				
				(*m_vFinalImage)[y][x][0] = (int)((*m_vPostProcImage)[y][x][0]);
				(*m_vFinalImage)[y][x][1] = (int)((*m_vPostProcImage)[y][x][1]);
				(*m_vFinalImage)[y][x][2] = (int)((*m_vPostProcImage)[y][x][2]);
				(*m_vFinalImage)[y][x][3] = (int)((*m_vPostProcImage)[y][x][3]);
			
				
				//if ((*m_vFinalImage)[y][x][3] != 255)
				/*if ((*m_vImage)[y][x][3] != 255)
				{
					cout << (*m_vImage)[y][x][3] << endl;
					cout << "WARNING - alpha isn't up" << endl;
				}*/
			}
		}
		pBar7.Finish();
	}

	void FlameFractal::SaveFunctionCode(string sFileName)
	{
		cout << "Saving function codes to " << sFileName << "_functions.xml..." << endl;
			
		// create the root node
		xml_document* pDoc = new xml_document();
		xml_node pFunctions = pDoc->append_child("Functions");
		
		// get the xml for each individual function
		for (int i = 0; i < m_vFunctions.size(); i++)
		{
			FFFunction pFunction = m_vFunctions[i];

			xml_document* pFunctionDoc = pFunction.GetFunctionXML();
			pFunctions.append_copy(pFunctionDoc->document_element());
		}
		
		// save it!
		pDoc->save_file((sFileName + "_functions.xml").c_str());

		cout << "Functions saved successfully!" << endl;
	}

	void FlameFractal::LoadFunctionCode(string sFileName)
	{
		cout << "Loading function code from " << sFileName << "_functions.xml..." << endl;
		
		// load xml from the file
		xml_document pDoc;
		pDoc.load_file((sFileName + "_functions.xml").c_str());

		// reset list of functions
		ClearFunctions();
		
		// load each individual function into this object
		for (xml_node pFunctionNode = pDoc.document_element().child("Function"); pFunctionNode; pFunctionNode = pFunctionNode.next_sibling("Function"))
		{
			FFFunction* pFunction = new FFFunction();
			pFunction->LoadFromXML(pFunctionNode);
			AddFunction(*pFunction);
		}

		cout << "Functions loaded successfully!" << endl;
	}

	void FlameFractal::SaveImageTrace(string sFileName)
	{
		cout << "Saving image trace..." << endl;
		
		// store meta
		string sSaveData = "";
		sSaveData += to_string(m_iWidth) + "\n";
		sSaveData += to_string(m_iHeight) + "\n";
		sSaveData += to_string(m_fTraceX) + "\n";
		sSaveData += to_string(m_fTraceY) + "\n";
		sSaveData += to_string(m_fTraceC) + "\n";
		sSaveData += to_string(m_fScalarX) + "\n";
		sSaveData += to_string(m_fScalarY) + "\n";
		sSaveData += to_string(m_fRawOffsetX) + "\n";
		sSaveData += to_string(m_fRawOffsetY) + "\n";
		sSaveData += m_sColorName + "\n";
		sSaveData += to_string(m_iIterations) + "\n";

		sSaveData += to_string(m_vRampPoints->size()) + "\n";

		for (int i = 0; i < m_vRampPoints->size(); i++) { sSaveData += to_string((*m_vRampPoints)[i]) + "\n"; }
		for (int i = 0; i < m_vRampVals->size(); i++)
		{
			sSaveData += to_string((*m_vRampVals)[i][0]) + "\n";
			sSaveData += to_string((*m_vRampVals)[i][1]) + "\n";
			sSaveData += to_string((*m_vRampVals)[i][2]) + "\n";
		}

		ProgressBar pBar = ProgressBar(m_vPoints->size() - 1, m_iProgressBarSize);
		
		for (int y = 0; y < m_vPoints->size(); y++)
		{
			pBar.Update(y);
			for (int x = 0; x < (*m_vPoints)[y].size(); x++)
			{
				float fR = (*m_vPoints)[y][x][0];
				float fG = (*m_vPoints)[y][x][1];
				float fB = (*m_vPoints)[y][x][2];
				float fA = (*m_vPoints)[y][x][3];

				sSaveData += to_string(fR) + "," + to_string(fG) + "," + to_string(fB) + "," + to_string(fA) + "\n";
			}
		}
		pBar.Finish();

		cout << "Writing file..." << endl;
		ofstream pFile;
		pFile.open((sFileName + "_trace.dat").c_str());
		pFile << sSaveData << endl;
		pFile.close();

		cout << "Trace saved successfully!" << endl;
	}

	void FlameFractal::LoadImageTrace(string sFileName)
	{
		cout << "Loading image trace..." << endl;
		cout << "(" << sFileName << ")" << endl; // DEBUG

		ifstream pFile;
		pFile.open((sFileName + "_trace.dat").c_str());

		pFile >> m_iWidth;
		pFile >> m_iHeight;
		pFile >> m_fStartX;
		pFile >> m_fStartY;
		pFile >> m_fStartC;
		pFile >> m_fScalarX;
		pFile >> m_fScalarY;
		pFile >> m_fRawOffsetX;
		pFile >> m_fRawOffsetY;
		pFile >> m_sColorName;
		pFile >> m_iIterations;

		int iColorPoints = 0;
		pFile >> iColorPoints;

		InitializeColorRamp(iColorPoints);
		
		for (int i = 0; i < iColorPoints; i++) { pFile >> (*m_vRampPoints)[i]; }
		for (int i = 0; i < iColorPoints; i++)
		{
			pFile >> (*m_vRampVals)[i][0];
			pFile >> (*m_vRampVals)[i][1];
			pFile >> (*m_vRampVals)[i][2];
		}
		

		int iX = 0;
		int iY = 0;

		ProgressBar pBar = ProgressBar(m_iHeight, m_iProgressBarSize);
		
		while (!pFile.eof())
		{
			string sNextLine;
			pFile >> sNextLine;
			//cout << sNextLine << endl;
			
			// split on comma
			vector<float>* vPoint = new vector<float>();
			stringstream ss(sNextLine);
			float fNum;
			while (ss >> fNum)
			{
				vPoint->push_back(fNum);
				if (ss.peek() == ',') { ss.ignore(); }
			}

			(*m_vPoints)[iY][iX] = *vPoint;
			iX++;
			if (iX == m_iWidth) { iX = 0; iY++; pBar.Update(iY); }
			if (iY == m_iHeight) { break; } // TODO: this really shouldn't be necessary, but it is? Double check logic somewhere here!
		}
		
		pBar.Finish();

		m_bInitialized = true;
		cout << "Image trace loaded!" << endl;
	}

	void FlameFractal::SaveImageData(string sFileName)
	{
		cout << "Saving image data..." << endl;

		ProgressBar pBar = ProgressBar(m_vFinalImage->size() - 1, m_iProgressBarSize);

		string sSaveData = "{\"width\": " + to_string(m_iWidth) + ", \"height\": " + to_string(m_iHeight) + ",";
		sSaveData += "\"pixels\":[";
		for (int y = 0; y < m_vFinalImage->size(); y++)
		{
			pBar.Update(y);
			sSaveData += "[";
			for (int x = 0; x < (*m_vFinalImage)[y].size(); x++)
			{
				int r = (*m_vFinalImage)[y][x][0];
				int g = (*m_vFinalImage)[y][x][1];
				int b = (*m_vFinalImage)[y][x][2];
				int a = (*m_vFinalImage)[y][x][3];

				sSaveData += "[" + to_string(r) + "," + to_string(g) + "," + to_string(b) + "," + to_string(a) + "]";
				if (x < (*m_vFinalImage)[y].size() - 1) { sSaveData += ","; }
			}
			sSaveData += "]";
			if (y < m_vFinalImage->size() - 1) { sSaveData += ","; }
		}
		pBar.Finish();
		sSaveData += "]}";

		cout << "Writing image data to file..." << endl;
		ofstream fFile;
		fFile.open(sFileName);
		fFile << sSaveData;
		fFile.close();
		cout << "Saving complete!" << endl;
	}
}

#endif // FF_CPP

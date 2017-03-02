//*************************************************************
//  File: Generator.cpp
//  Date created: 1/28/2017
//  Date edited: 2/23/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************


#include <iostream>

#include <random>
#include <cmath>
#include <vector>
#include <string>
//#include <cstdlib>
//#include <ctime>

#include <fstream>

#include "Function.h"
#include "FlameFractal.h"
#include "ProgressBar.h"
#include "FunctionGenerator.h"

#define PI 3.14159265

using namespace std;
using namespace dwl;

int main()
{
	FlameFractal ff = FlameFractal(1000, 1000);
	
	int iCollection = 22;

	/*FunctionGenerator pGen = FunctionGenerator();
	FFFunction* pF0 = pGen.GenerateFunction();
	ff.AddFunction(*pF0);
	FFFunction* pF1 = pGen.GenerateFunction();
	ff.AddFunction(*pF1);
	FFFunction* pF2 = pGen.GenerateFunction();
	ff.AddFunction(*pF2);
	FFFunction* pF3 = pGen.GenerateFunction();
	ff.AddFunction(*pF3);

	FFFunction* pFSym = pGen.GenerateSymmetryFunction(120);
	pFSym->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
	ff.AddFunction(*pFSym);
	FFFunction* pFSym2 = pGen.GenerateSymmetryFunction(240);
	pFSym2->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
	ff.AddFunction(*pFSym2);*/
	
	// colors
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 0.5f, 1.0f}}); // nice blue!
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.5f}}); // greeen?
	//ff.SetColorRamp({0.0f, 0.5f, 1.0f}, {{1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.7f, 0.4f, 1.0f}}); // TTU!
	//ff.SetColorRamp({0.0f, 0.15f, 0.3f, 0.55f, 0.7f, 1.0f}, {{1.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.5f, 1.0f}, {0.8f, 0.2f, 1.0f}});
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.7f, 0.4f, 1.0f}}); // purple
	//ff.SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}}); // deep purple
	ff.SetColorRamp({0.0f, 0.5f, 1.0f}, {{0.0f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}}); // purple'n blue
	

	ff.SetZoom(.4, .4);
	ff.PreparePlot();
	ff.InitializeSolution(); 

	ff.LoadFunctionCode("collection/" + to_string(iCollection));
	//ff.LoadImageTrace("collection/" + to_string(iCollection));
	//ff.SaveFunctionCode("collection/" + to_string(iCollection));
	
	ff.Solve(500000000);
	ff.SaveImageTrace("collection/" + to_string(iCollection));
	
	// render
	ff.Render(2.8, 1.2, 0);
	ff.SaveImageData("imgdata.json");
	system("python3 ./saveaspng.py");
	string sCopyCommand = "cp render.png collection/" + to_string(iCollection) + "_render.png";
	system(sCopyCommand.c_str());
	

	/*int iPart = 1;
	
	// part 1
	if (iPart == 1)
	{
		ff.PreparePlot();
		ff.InitializeSolution();
		ff.Solve(10000000);
		ff.Render(2.2, 1.0, 0);
		ff.SaveImageTrace("Test");
		ff.SaveImageData("imgdata.json");
		ff.SaveFunctionCode("TestFunctions");
		system("python3 ./saveaspng.py");
	}
	
	// part 2
	if (iPart == 2)
	{
		ff.PreparePlot();
		ff.LoadImageTrace("Test");
		ff.Solve(100000000);
		ff.Render(2.2, 1.0, 0);
		ff.SaveImageTrace("Test");
		ff.SaveImageData("imgdata.json");
		system("python3 ./saveaspng.py");
	}*/
	
	//ff.SetBaseImage(100.0f, 0.0f, 0.0f, 255.0f);
	//ff.LoadFunctionCode("Test");
	//ff.SaveImageTrace("Test");
	//ff.LoadImageTrace("Test");

	//SaveImage("imgdata.json", &ff);
	
	
	/*random_device rd;
	srand(rd());
	float fRandom = ((float) rand()) / (float) RAND_MAX;

	cout << "Random: " << fRandom << endl;*/


	//FFFunction f = FFFunction();
	//f.SetVariationWeight(FFFunction::VAR_LINEAR, 1.0f);
	//cout << "Running function..." << endl;
	////vector<float> result = f.Run(1.0, 1.0);
	//f.Run(1.0, 1.0);
	//float x = f.GetResultX();
	//float y = f.GetResultY();
	//cout << "x: " << x << " y: " << y << endl;

	//for (int i = 0; i < result.size(); i++)
	//{
		//cout << result[i] << endl;
	//}

	
	return 0;
}

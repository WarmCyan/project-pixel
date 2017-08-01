//*************************************************************
//  File: Generator.cpp
//  Date created: 1/28/2017
//  Date edited: 3/1/2017
//  Author: Nathan Martindale
//  Copyright © 2017 Digital Warrior Labs
//  Description: 
//*************************************************************


#include <iostream>

#include <cmath>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include "FlameFractal.h"
#include "FunctionGenerator.h"

using namespace std;
using namespace dwl;

void REPL();
int HandleCommand(string sCommand);

void StoreCollectionNum();
void LoadCollectionNum();

FlameFractal* pFractal = new FlameFractal(0,0);
int iCollection = 0;
string sErrorMsg = "";

int main()
{
	LoadCollectionNum();
	cout << "Collection: " << iCollection << endl;

	REPL();
	return 0;
	
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
	
	ff.Solve(10000000);
	//ff.SaveImageTrace("collection/" + to_string(iCollection));
	
	// render
	ff.Render(2.8, 1.2, 0);
	ff.SaveImageData("imgdata.json");
	system("python3 ./saveaspng.py");
	string sCopyCommand = "copy \"./render.png\" \"./collection/" + to_string(iCollection) + "_render.png\""; // TODO: make cross platform!!
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

void StoreCollectionNum()
{
	cout << "Storing collection num..." << endl;
	ofstream fFile;
	fFile.open("./collection/collection", ios::out);
	fFile << iCollection;
	fFile.close();
}

void LoadCollectionNum()
{
	cout << "Loading collection num..." << endl;
	ifstream fFile;
	fFile.open("./collection/collection");
	fFile >> iCollection;
	fFile.close();
}

void REPL()
{
	int iResult = 0;
	while (iResult != 2)
	{
		cout << "Pixel> ";
		string sCommand = "";
		//cin >> sCommand;
		getline(cin, sCommand);
		iResult = HandleCommand(sCommand);
		if (iResult == 1) { cout << "ERROR: " << sErrorMsg << endl; }
	}
}

// 0 = normal, 1 = error, 2 = exit
int HandleCommand(string sCommand)
{
	cout << "" << sCommand << endl;

	// split on space (black magic split)
	stringstream ss(sCommand);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> vParts(begin, end);

	if (vParts.size() == 0) { return 0; cin.ignore(); cin.clear(); }
	if (vParts[0][0] == '#') { return 0; }
	
	if (vParts[0] == "exit")
	{
		cout << "Have a nice day!" << endl;
		return 2;
	}
	else if (vParts[0] == "help")
	{
		cout << "exit\necho [MESSAGE]\ncollection [INDEX|save|load]\nrun [SCRIPT]\ncreate [WIDTH] [HEIGHT]\nzoom [X] [Y]\ncolor [blue|green|ttu|purple|purpleblue|orange|yellow|red|portal]\ninit\nsolve [COUNT]\nrender [GAMMA] [BRIGHTNESS]\ngenerate\nsave [image|functions|trace|collection] {FILE}\nload [functions|trace] [FILE]" << endl;
		return 0;
	}
	else if (vParts[0] == "run")
	{
		if (vParts.size() != 2)
		{
			sErrorMsg = "Bad arguments!nFORMAT: run [SCRIPT]";
			return 1;
		}

		ifstream pFile;
		pFile.open(vParts[1].c_str());

		cout << "Running '" << vParts[1] << "'..." << endl;
		while (!pFile.eof())
		{
			string sLine;
			//pFile >> sLine;
			getline(pFile, sLine);

			int iResult = HandleCommand(sLine);
			if (iResult == 1) { pFile.close(); return 1; }
		}
		cout << "'" << vParts[1] << "' finished executing" << endl;
		
		pFile.close();
		return 0;
	}
	else if (vParts[0] == "create")
	{
		if (vParts.size() != 3) 
		{ 
			sErrorMsg = "Bad arguments!\nFORMAT: create [WIDTH] [HEIGHT]";
			return 1;
		}
		//float fWidth = (float)vParts[1];
		//float fHeight = (float)vParts[2];
		int iWidth = stoi(vParts[1]);
		int iHeight = stoi(vParts[2]);
		cout << "Parsed " << "[Width: " << iWidth << "] [Height: " << iHeight << "]" << endl;
		
		delete pFractal;
		pFractal = new FlameFractal(iWidth, iHeight);
		pFractal->PreparePlot();
		
		return 0;
	}
	else if (vParts[0] == "init") { pFractal->InitializeSolution(); return 0; }
	else if (vParts[0] == "zoom")
	{
		if (vParts.size() != 3) 
		{ 
			sErrorMsg = "Bad arguments!\nFORMAT: zoom [XSCALAR] [YSCALAR]";
			return 1;
		}
		//float fWidth = (float)vParts[1];
		//float fHeight = (float)vParts[2];
		float fX = stof(vParts[1]);
		float fY = stof(vParts[2]);
		cout << "Parsed " << "[X: " << fX << "] [Y: " << fY << "]" << endl;
		
		pFractal->SetZoom(fX, fY);
		
		return 0;
	}

	else if (vParts[0] == "collection")
	{
		if (vParts.size() != 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: collection [INDEX|save|load]";
			return 1;
		}

		if (vParts[1] == "save")
		{
			
		}

		int iIndex = stoi(vParts[1]);
		cout << "Parsed " << "[Index: " << iIndex << "]" << endl;
		iCollection = iIndex;
		return 0;
	}

	else if (vParts[0] == "color")
	{
		if (vParts.size() != 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: color [COLORRAMPNAME]";
			return 1;
		}

		cout << "Parsed " << "[Color: " << vParts[1] << "]" << endl;

		bool bFound = false;
		if (vParts[1] == "blue")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 0.5f, 1.0f}}); // nice blue!
			bFound = true;
		}
		else if (vParts[1] == "green")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.5f}}); // greeen
			bFound = true;
		}
		else if (vParts[1] == "ttu")
		{
			pFractal->SetColorRamp({0.0f, 0.5f, 1.0f}, {{1.0f, 1.0f, 0.0f}, {0.5f, 0.5f, 0.5f}, {0.7f, 0.4f, 1.0f}}); // TTU!
			bFound = true;
		}
		else if (vParts[1] == "purple")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {0.7f, 0.4f, 1.0f}}); // purple
			bFound = true;
		}
		else if (vParts[1] == "purpleblue")
		{
			pFractal->SetColorRamp({0.0f, 0.5f, 1.0f}, {{0.0f, 0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.5f, 0.2f, 0.7f}}); // purple'n blue
			bFound = true;
		}
		else if (vParts[1] == "orange")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {1.0f, 0.5f, 0.0f}});
			bFound = true;
		}
		else if (vParts[1] == "yellow")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}});
			bFound = true;
		}
		else if (vParts[1] == "red")
		{
			pFractal->SetColorRamp({0.0f, 1.0f}, {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
			bFound = true;
		}
		else if (vParts[1] == "portal")
		{
			pFractal->SetColorRamp({0.0f, 0.5f, 1.0f}, {{1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.5f, 1.0f}});
			bFound = true;
		}

		if (!bFound)
		{
			sErrorMsg = "Could not find colorscheme '" + vParts[1] + "'";
			return 1;
		}
		//cout << "Set color ramp!" << endl;
		return 0;
	}

	else if (vParts[0] == "generate")
	{
		cout << "Generating random functions..." << endl;
		FunctionGenerator pGen = FunctionGenerator();
		FFFunction* pF0 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF0);
		FFFunction* pF1 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF1);
		FFFunction* pF2 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF2);
		FFFunction* pF3 = pGen.GenerateFunction();
		pFractal->AddFunction(*pF3);

		FFFunction* pFSym = pGen.GenerateSymmetryFunction(120);
		pFSym->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
		pFractal->AddFunction(*pFSym);
		FFFunction* pFSym2 = pGen.GenerateSymmetryFunction(240);
		pFSym2->SetWeight(pF0->GetWeight() + pF1->GetWeight() + pF2->GetWeight() + pF3->GetWeight());
		pFractal->AddFunction(*pFSym2);

		cout << "Generated!" << endl;
		return 0;
	}

	else if (vParts[0] == "load")
	{
		if (vParts.size() < 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: load [functions|trace] {FILE}";
			return 1;
		}
		if (vParts[1] == "functions")
		{
			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "collection/" + to_string(iCollection); }
			else { sFileName = vParts[2]; }

			pFractal->LoadFunctionCode(sFileName);
			return 0;
		}
		if (vParts[1] == "trace")
		{
			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "collection/" + to_string(iCollection); }
			else { sFileName = vParts[2]; }
			
			pFractal->LoadImageTrace(sFileName);
			return 0;
		}
		if (vParts[1] == "collection") { LoadCollectionNum(); return 0; }
	}

	else if (vParts[0] == "solve")
	{
		if (vParts.size() != 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: solve [COUNT]";
			return 1;
		}

		int iSamples = stoi(vParts[1]);
		cout << "Parsed [Samples: " << iSamples << "]" << endl;
		pFractal->Solve(iSamples);
		return 0;
	}

	else if (vParts[0] == "render")
	{
		if (vParts.size() != 3)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: render [GAMMA] [BRIGHTNESS]";
			return 1;
		}

		float fGamma = stof(vParts[1]);
		float fBrightness = stof(vParts[2]);
		cout << "Parsed [Gamma: " << fGamma << "] [Brightness: " << fBrightness << "]" << endl;
		pFractal->Render(fGamma, fBrightness, 0);
		return 0;
	}

	else if (vParts[0] == "save")
	{
		if (vParts.size() < 2)
		{
			sErrorMsg = "Bad arguments!\nFORMAT: save [image|functions|trace|collection] {FILE}";
			return 1;
		}

		if (vParts[1] == "image")
		{
			pFractal->SaveImageData("imgdata.json");
			system("python3 ./saveaspng.py");

			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "./collection/" + to_string(iCollection) + "_render.png"; }
			else { sFileName = vParts[2]; }
			
			string sCopyCommand = "copy \"./render.png\" \"" + sFileName + "\"";
			
			cout << "Copying to " << sFileName << "..." << endl;

			system(sCopyCommand.c_str());
			return 0;
		}
		else if (vParts[1] == "functions")
		{
			string sFileName = "";
			
			if (vParts.size() == 2) { sFileName = "./collection/" + to_string(iCollection); }
			else { sFileName = vParts[2]; }
			
			pFractal->SaveFunctionCode(sFileName);
			return 0;
		}
		else if (vParts[1] == "trace")
		{
			string sFileName = "";

			if (vParts.size() == 2) { sFileName = "./collection/" + to_string(iCollection); }
			else { sFileName = vParts[2]; }

			pFractal->SaveImageTrace(sFileName);
			return 0;
		}
		else if (vParts[1] == "collection") { StoreCollectionNum(); return 0; }
	}

	else if (vParts[0] == "echo") // for debugging use!
	{
		cout << endl << "========================================" << endl << endl;
		for (int i = 1; i < vParts.size(); i++)
		{
			cout << vParts[i] << " ";
		}
		cout << endl << endl << "========================================" << endl << endl;
		return 0;
	}
	
	sErrorMsg = "Unrecognized command!";
	return 1;
}

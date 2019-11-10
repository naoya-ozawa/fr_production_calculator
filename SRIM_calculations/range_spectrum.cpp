#include <iostream>
#include <bits/stdc++.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TH1D.h>
#include <TF1.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

int main(int argc, char** argv){
  TRint rootapp("app",&argc,argv);
  TCanvas *c1 = new TCanvas();
//	c1->Divide(2,1);

  fstream file;
  string word, filename;

  filename = "./EXYZ_AuMoTarget_70MeV.txt";
//  filename = "./EXYZ_AuMoTarget_80MeV.txt";
//  filename = "./EXYZ_AuMoTarget_90MeV.txt";
//  filename = "./EXYZ_AuMoTarget_100MeV.txt";
//  filename = "./EXYZ_AuMoTarget_110MeV.txt";
//  filename = "./EXYZ_AuMoTarget_120MeV.txt";
//  filename = "./EXYZ_AuMoTarget_130MeV.txt";
//  filename = "./EXYZ_AuMoTarget_130MeV_tester.txt";

	file.open(filename.c_str());

//  double threshold = 120.0; // MeV
//  double threshold = 110.0; // MeV
//  double threshold = 100.0; // MeV
//  double threshold = 90.0; // MeV
//  double threshold = 80.0; // MeV
//  double threshold = 70.0; // MeV
  double threshold = 0.1; // MeV
  cout << "Examine range for threshold " << threshold << " MeV" << endl;

	TH1D *spectrum = new TH1D("spectrum",Form("%3.f MeV Range of 70 MeV {}^{18}O Ions Injected into the Au Target",threshold),4000,0.,40.);
	spectrum->GetXaxis()->SetTitle("Range (#mum)");
	spectrum->GetYaxis()->SetTitle("Ions / 0.01 #mum");

  int Nion = 0;

	int dataflag = 0;
	int datacol = 0;
  int newion = 0; // 0: record data, 1: has been recorded

  double prevDepth = 0.0;

	double mean = 0.0;
	double stdev = 0.0;

	while (file >> word){
//		cout << word << endl;
		try{
			if (word == "0000001") dataflag = 1;
			if (dataflag == 1){
				++datacol;
//        cout << "datacol = " << datacol << ", ";
				if (datacol%7 == 1){
          if (stoi(word) != Nion){
            newion = 1; // this is a new ion
//            cout << "Finished analyzing ion #" << Nion << endl;
          }
				  Nion = stoi(word);
				}else if (datacol%7 == 2){
					double energy = stod(word)*TMath::Power(10.,-3); // MeV
          if ((energy < threshold)&&(newion == 1)){
            double range = prevDepth;
            spectrum->Fill(range);
            mean += range;
            stdev += range*range;
            newion = 0;
          }
				}else if (datacol%7 == 3){
				  double depth = stod(word)*TMath::Power(10.,-4); // um
          prevDepth = depth;
				}else if (datacol%7 == 4){
					double spreadY = stod(word)*TMath::Power(10.,-4); // um
				}else if (datacol%7 == 5){
					double spreadZ = stod(word)*TMath::Power(10.,-4); // um
				}else if (datacol%7 == 6){
          double electronicStop = stod(word); // eV/A
        }else{
					double prevCollision = stod(word)*TMath::Power(10.,-6); // MeV (energy lost in previous collision)
				}
			}
		}
		catch (const invalid_argument& e){
			cout << "EOF" << endl;
		}
	}

	cout << Nion << " ions analyzed" << endl;

  spectrum->Draw();

  mean /= double(Nion);
  cout << "Mean Range = " << mean << " um" << endl;
  stdev /= double(Nion);
  stdev = TMath::Sqrt(stdev - (mean*mean));
  cout << "StDev = " << stdev << " um" << endl;

  TLatex *l = new TLatex();
  l->SetTextSize(0.05);
  l->DrawLatex(25.,200.,Form("E[X_{%3.f MeV}] = %3.2f #mum",threshold,mean));
  l->DrawLatex(25.,150.,Form("#sigma[X_{%3.f MeV}] = %3.2f #mum",threshold,stdev));

  rootapp.Run();

  c1->Update();
  c1->Modified();

  return 0;

}

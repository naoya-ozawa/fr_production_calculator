#include <iostream>
#include <bits/stdc++.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

int main(int argc, char** argv){

  TRint rootapp("app",&argc,argv);
  TCanvas *c1 = new TCanvas();

  fstream file;
  string word, filename;

  filename = "./O_in_Au_dEdX.txt";

	file.open(filename.c_str());

  TGraphErrors *ge_dedx = new TGraphErrors();
  ge_dedx->SetTitle("Stopping Range of {}^{18}O in Au Target;Incident Beam Energy (MeV);Stopping Range (MeV/(mg/cm^{2}))");

	int dataflag = 0;
	int datacol = 0;
  int newion = 0; // 0: record data, 1: has been recorded

  double energy = -9999.;
  string unit = "eV";
  double dedx_elec = -9999.;
  double dedx_nucl = -9999.;

	while (file >> word){
//		cout << word << endl;
		try{
			if (word == "10.00") dataflag = 1;
      if (word == "-----------------------------------------------------------") dataflag = 0;
			if (dataflag == 1){
				++datacol;
//        cout << "datacol = " << datacol << ", ";
				if (datacol%10 == 1){
				  energy = stod(word);
				}else if (datacol%10 == 2){
          unit = word;
          if (unit == "keV"){
            energy *= TMath::Power(10.,-3); // keV --> MeV
          }
				}else if (datacol%10 == 3){
				  dedx_elec = stod(word); // MeV/(mg/cm^2)
				}else if (datacol%10 == 4){
          dedx_nucl = stod(word);
				}else if (datacol%10 == 5){
					double projected_range = stod(word)*TMath::Power(10.,-4); // um
				}else if (datacol%10 == 7){
          double longitudinal_straggling = stod(word)*TMath::Power(10.,-4); // um
        }else if (datacol%10 == 9){
          double lateral_straggling = stod(word)*TMath::Power(10.,-4); // um
          ge_dedx->SetPoint((datacol-1)/10,energy,dedx_elec+dedx_nucl);
        }
			}
		}
		catch (const invalid_argument& e){
			cout << "EOF" << endl;
		}
	}

  ge_dedx->SetLineColor(4);
  ge_dedx->SetLineWidth(2);
  ge_dedx->Draw("AL");

  rootapp.Run();

  c1->Update();
  c1->Modified();

  return 0;

}

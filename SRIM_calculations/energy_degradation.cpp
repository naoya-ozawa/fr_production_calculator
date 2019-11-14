#include <bits/stdc++.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

double energy_loss(double C_E0, double C_E1, double C_Be, double C_He, double E_incident, double T_Be, double T_He){
	return C_E0 + (C_E1/E_incident) + C_Be*T_Be + C_He*T_He;
}

double degraded_energy(double *x,double *par){
	double E_incident = x[0];
	double T_Be = par[0];
	double T_He = par[1];

	// Fitted parameters from the dependence_plotter.cpp
	double C_E0 = -8.76; // MeV
	double C_E1 = 828.91; // MeV*MeV_incident
	double C_Be = 0.68; // MeV/um_Be
	double C_He = 0.0875; // MeV/mm_He

	return E_incident - energy_loss(C_E0,C_E1,C_Be,C_He,E_incident,T_Be,T_He);
}

int main(int argc, char** argv){
	TRint rootapp("app",&argc,argv);
	TCanvas *c1 = new TCanvas();

	double T_Be_03 = 11.3; // um
	double T_Be_04 = 9.6; // um
	double T_Be_max = 15.0 + 13.0; // um

	double T_He = 4.2 + 17.5 + 6.7; // mm

	TF1 *f_degraded_beam = new TF1(Form("%g #mum Be + %g mm He + %g #mum Be",T_Be_03,T_He,T_Be_04),degraded_energy,95.,135.,2);
	f_degraded_beam->SetParameters(T_Be_03+T_Be_04,T_He);
	TGraph *g_degraded_beam = new TGraph(f_degraded_beam);
	g_degraded_beam->SetMarkerColor(2);
	g_degraded_beam->SetLineColor(2);

	TF1 *f_thickBe = new TF1("15 #mum Be + 28.4 mm He + 13 #mum Be",degraded_energy,95.,135.,2);
	f_thickBe->SetParameters(T_Be_max,T_He);
	TGraph *g_thickBe = new TGraph(f_thickBe);
	g_thickBe->SetMarkerColor(4);
	g_thickBe->SetLineColor(4);

	TMultiGraph *mg_degraded_beam = new TMultiGraph();
	mg_degraded_beam->SetTitle("{}^{18}O Beam Energy Degradation through the Be Window;Incident Energy E_{incident} (MeV);Outgoing Beam Energy E_{out}");
	mg_degraded_beam->Add(g_degraded_beam);
	mg_degraded_beam->Add(g_thickBe);

	mg_degraded_beam->Draw("ALP");

	c1->BuildLegend();

  rootapp.Run();
  c1->Update();
  c1->Modified();

  return 0;
}

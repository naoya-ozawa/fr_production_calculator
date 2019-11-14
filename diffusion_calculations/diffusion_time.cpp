#include <bits/stdc++.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

double escape_probability(double *x, double *par){
  // Fujioka1981, assuming foil of thickness 2d
  double t = x[0]; // s
  double d = par[0]; // cm
  double D = par[1]; // cm^2 s^-1

  double tauD = d*d/(TMath::Pi()*TMath::Pi()*D);
  double t_tilde = t/tauD;

  double f = 0.0;
  for (int i=1; i<10000; ++i){
    double m = double(i);
    f += TMath::Exp(-(m-0.5)*(m-0.5)*t_tilde) / ((m-0.5)*(m-0.5));
  }
  f *= 2.0/(TMath::Pi()*TMath::Pi());

  return 1.0 - f;
}

double Au_diffusion(double D_0_1, double Q_1, double D_0_2, double Q_2, double T){
  double Tm = 1338.0; // melting point (K)

  if (T >= Tm){ // Lu2006
    double D_Tm = 2.50*TMath::Power(10.0,-9); // m^2/s
    double M = 197.0; // g/mol
    double gamma_lv = 1.21; // J/m^2
    double r = 0.82*TMath::Power(10.0,-10); // m
    double p = 0.13;
    double q = -0.12;

    return D_Tm*TMath::Power(T/Tm,3./2.) / ( (1.+p-(p*T/Tm)) * TMath::Power(1.-q+(q*T/Tm),2./3.) );
  }else{ // Neumann1986
    double kB = 8.6*TMath::Power(10.,-5); // eV/K

    return D_0_1*TMath::Exp(-Q_1/(kB*T)) + D_0_2*TMath::Exp(-Q_2/(kB*T)); // cm^2 s^-1
  }
}


int main(int argc, char** argv){
	TRint rootapp("app",&argc,argv);
	TCanvas *c1 = new TCanvas();
	c1->Divide(2,1);

  double temp[4] = {300.,600.,900.,1200.}; // degC
  double inject_energy[6] = {80.,90.,100.,110.,120.,130.};
  double depth_EXYZ70MeV[6] = {2.7,5.9,9.2,12.7,16.3,20.1}; // um depth for the 70 MeV range
  for (int i=0; i<6; ++i){
    depth_EXYZ70MeV[i] *= TMath::Power(10.,-4); // um --> cm
  }

  double Au_D01 = 0.025; // cm^2 s^-1
  double Au_Q1 = 1.7; // eV
  double Au_D02 = 0.83; // cm^2 s^-1
  double Au_Q2 = 2.2; // eV

  double D_Fr[4];
  for (int i=0; i<4; ++i){
    // Schoen1958
    D_Fr[i] = TMath::Sqrt(197./210.)*Au_diffusion(Au_D01,Au_Q1,Au_D02,Au_Q2,temp[i]+273.);
  }

  c1->cd(1);
  // escape probability vs time for various temperatures

  double depth = 9.2*TMath::Power(10.,-4); // 100 MeV injection

  TF1 *f_surface_Fr_300deg = new TF1("300#circC",escape_probability,0.0,300.0,2);
  f_surface_Fr_300deg->SetParameters(depth,D_Fr[0]);
  TGraph *g_surface_Fr_300deg = new TGraph(f_surface_Fr_300deg);
  g_surface_Fr_300deg->SetMarkerColor(9);
  g_surface_Fr_300deg->SetLineColor(9);

  TF1 *f_surface_Fr_600deg = new TF1("600#circC",escape_probability,0.0,300.0,2);
  f_surface_Fr_600deg->SetParameters(depth,D_Fr[1]);
  TGraph *g_surface_Fr_600deg = new TGraph(f_surface_Fr_600deg);
  g_surface_Fr_600deg->SetMarkerColor(4);
  g_surface_Fr_600deg->SetLineColor(4);

  TF1 *f_surface_Fr_900deg = new TF1("900#circC",escape_probability,0.0,300.0,2);
  f_surface_Fr_900deg->SetParameters(depth,D_Fr[2]);
  TGraph *g_surface_Fr_900deg = new TGraph(f_surface_Fr_900deg);
  g_surface_Fr_900deg->SetMarkerColor(3);
  g_surface_Fr_900deg->SetLineColor(3);

  TF1 *f_surface_Fr_1200deg = new TF1("1200#circC",escape_probability,0.0,300.0,2);
  f_surface_Fr_1200deg->SetParameters(depth,D_Fr[3]);
  TGraph *g_surface_Fr_1200deg = new TGraph(f_surface_Fr_1200deg);
  g_surface_Fr_1200deg->SetMarkerColor(2);
  g_surface_Fr_1200deg->SetLineColor(2);

  TMultiGraph *mg_escape = new TMultiGraph();
  mg_escape->SetTitle("{}^{210}Fr Escape Probability from Depth 9.2 #mum in the Au;Time (s);Probability");
  mg_escape->Add(g_surface_Fr_1200deg);
  mg_escape->Add(g_surface_Fr_900deg);
  mg_escape->Add(g_surface_Fr_600deg);
  mg_escape->Add(g_surface_Fr_300deg);

  mg_escape->Draw("ALP");
  c1->cd(1)->BuildLegend();

  c1->cd(2);

  // average escape time vs beam energy for various temperatures
  // using x_Ave = sqrt(2Dt) & 2sqrt(Dt)
  // eq to t_Ave = d^2/2D & d^2/4D

  double D_Fr_300deg = D_Fr[0];
  double D_Fr_600deg = D_Fr[1];
  double D_Fr_900deg = D_Fr[2];
  double D_Fr_1200deg = D_Fr[3];

  double tAve_nrm_300deg[6];
  double tAve_nrm_600deg[6];
  double tAve_nrm_900deg[6];
  double tAve_nrm_1200deg[6];
  double tAve_erf_300deg[6];
  double tAve_erf_600deg[6];
  double tAve_erf_900deg[6];
  double tAve_erf_1200deg[6];
  for (int i=0; i<6; ++i){
    tAve_nrm_300deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(2.0*D_Fr_300deg);
    tAve_nrm_600deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(2.0*D_Fr_600deg);
    tAve_nrm_900deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(2.0*D_Fr_900deg);
    tAve_nrm_1200deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(2.0*D_Fr_1200deg);
    tAve_erf_300deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(4.0*D_Fr_300deg);
    tAve_erf_600deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(4.0*D_Fr_600deg);
    tAve_erf_900deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(4.0*D_Fr_900deg);
    tAve_erf_1200deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i]/(4.0*D_Fr_1200deg);
  }

  TGraph *g_nrm_300deg = new TGraph(6,inject_energy,tAve_nrm_300deg);
  g_nrm_300deg->SetTitle("300#circC, #sqrt{2Dt} model");
  g_nrm_300deg->SetMarkerColor(9);
  g_nrm_300deg->SetLineColor(9);

  TGraph *g_nrm_600deg = new TGraph(6,inject_energy,tAve_nrm_600deg);
  g_nrm_600deg->SetTitle("600#circC, #sqrt{2Dt} model");
  g_nrm_600deg->SetMarkerColor(4);
  g_nrm_600deg->SetLineColor(4);

  TGraph *g_nrm_900deg = new TGraph(6,inject_energy,tAve_nrm_900deg);
  g_nrm_900deg->SetTitle("900#circC, #sqrt{2Dt} model");
  g_nrm_900deg->SetMarkerColor(3);
  g_nrm_900deg->SetLineColor(3);

  TGraph *g_nrm_1200deg = new TGraph(6,inject_energy,tAve_nrm_1200deg);
  g_nrm_1200deg->SetTitle("1200#circC, #sqrt{2Dt} model");
  g_nrm_1200deg->SetMarkerColor(2);
  g_nrm_1200deg->SetLineColor(2);

  TGraph *g_erf_300deg = new TGraph(6,inject_energy,tAve_erf_300deg);
  g_erf_300deg->SetTitle("300#circC, 2#sqrt{Dt} model");
  g_erf_300deg->SetMarkerColor(9);
  g_erf_300deg->SetLineColor(9);
  g_erf_300deg->SetLineStyle(2);

  TGraph *g_erf_600deg = new TGraph(6,inject_energy,tAve_erf_600deg);
  g_erf_600deg->SetTitle("600#circC, 2#sqrt{Dt} model");
  g_erf_600deg->SetMarkerColor(4);
  g_erf_600deg->SetLineColor(4);
  g_erf_600deg->SetLineStyle(2);

  TGraph *g_erf_900deg = new TGraph(6,inject_energy,tAve_erf_900deg);
  g_erf_900deg->SetTitle("900#circC, 2#sqrt{Dt} model");
  g_erf_900deg->SetMarkerColor(3);
  g_erf_900deg->SetLineColor(3);
  g_erf_900deg->SetLineStyle(2);

  TGraph *g_erf_1200deg = new TGraph(6,inject_energy,tAve_erf_1200deg);
  g_erf_1200deg->SetTitle("1200#circC, 2#sqrt{Dt} model");
  g_erf_1200deg->SetMarkerColor(2);
  g_erf_1200deg->SetLineColor(2);
  g_erf_1200deg->SetLineStyle(2);

  TMultiGraph *mg_tAve = new TMultiGraph();
  mg_tAve->SetTitle("Average Escape Time of {}^{210}Fr from Au (from the 70 MeV depth);Incident {}^{18}O Beam Energy (MeV);Escape Time (s)");

  mg_tAve->Add(g_nrm_300deg);
  mg_tAve->Add(g_erf_300deg);
  mg_tAve->Add(g_nrm_600deg);
  mg_tAve->Add(g_erf_600deg);
  mg_tAve->Add(g_nrm_900deg);
  mg_tAve->Add(g_erf_900deg);
  mg_tAve->Add(g_nrm_1200deg);
  mg_tAve->Add(g_erf_1200deg);

  mg_tAve->Draw("ALP");
  c1->cd(2)->BuildLegend();

  rootapp.Run();

  c1->Update();
  c1->Modified();


  return 0;
}

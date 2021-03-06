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

double Au_selfdiffusion(double T){

  double Tm = 1338.0; // melting point (K)
  double D_0_1 = 0.025; // cm^2 s^-1
  double Q_1 = 1.7; // eV
  double D_0_2 = 0.83; // cm^2 s^-1
  double Q_2 = 2.2; // eV

  if (T >= Tm){ // Lu2006
    double D_Tm = 2.50*TMath::Power(10.0,-9); // m^2/s
//    double M = 197.0; // g/mol
//    double gamma_lv = 1.21; // J/m^2
//    double r = 0.82*TMath::Power(10.0,-10); // m
    double p = 0.13;
    double q = -0.12;

    return D_Tm*TMath::Power(10.,4) * TMath::Power(T/Tm,3./2.) / ( (1.+p-(p*T/Tm)) * TMath::Power(1.-q+(q*T/Tm),2./3.) );
  }else{ // Neumann1986
    double kB = 8.6*TMath::Power(10.,-5); // eV/K

    return D_0_1*TMath::Exp(-Q_1/(kB*T)) + D_0_2*TMath::Exp(-Q_2/(kB*T)); // cm^2 s^-1
  }
}

double D_m(double *x, double *par){
  double T = x[0]; // K
  double m = par[0]; // u
  double m_Au = 197.0; // u

  // Schoen1958
  return TMath::Sqrt(m_Au/m)*Au_selfdiffusion(T);
}

double y_escape(double lifetime, double D, double d){
  double alpha = lifetime * D / (d*d);
  return 0.5 * TMath::Sqrt(alpha) * TMath::TanH(1./TMath::Sqrt(alpha));
}


int main(int argc, char** argv){
	TRint rootapp("app",&argc,argv);
	TCanvas *c1 = new TCanvas();
	c1->Divide(2,2);

  double temp[4] = {300.,600.,900.,1200.}; // degC
  double inject_energy[6] = {80.,90.,100.,110.,120.,130.};
  double depth_EXYZ70MeV[6] = {2.7,5.9,9.2,12.7,16.3,20.1}; // um depth for the 70 MeV range
  double depth_EXYZ90MeV[6] = {1.0,1.0,3.03,6.50,10.13,13.92}; // um depth for the 90 MeV range --> maximize 210Fr production
  for (int i=0; i<6; ++i){
    depth_EXYZ70MeV[i] *= TMath::Power(10.,-4); // um --> cm
    depth_EXYZ90MeV[i] *= TMath::Power(10.,-4);
  }

  double D_Fr[4];
  for (int i=0; i<4; ++i){
    double temperature[1] = {temp[i]+273.};
    double mass[1] = {210.};
    D_Fr[i] = D_m(temperature,mass);
  }

  c1->cd(1);
  // escape probability vs time for various temperatures

  double depth = 3.03*TMath::Power(10.,-4); // 100 MeV injection, 210-Fr production 90 MeV range

  TF1 *f_surface_Fr_300deg = new TF1("300#circC",escape_probability,0.0,90.0,2);
  f_surface_Fr_300deg->SetParameters(depth,D_Fr[0]);
  TGraph *g_surface_Fr_300deg = new TGraph(f_surface_Fr_300deg);
  g_surface_Fr_300deg->SetMarkerColor(9);
  g_surface_Fr_300deg->SetLineColor(9);

  TF1 *f_surface_Fr_600deg = new TF1("600#circC",escape_probability,0.0,90.0,2);
  f_surface_Fr_600deg->SetParameters(depth,D_Fr[1]);
  TGraph *g_surface_Fr_600deg = new TGraph(f_surface_Fr_600deg);
  g_surface_Fr_600deg->SetMarkerColor(4);
  g_surface_Fr_600deg->SetLineColor(4);

  TF1 *f_surface_Fr_900deg = new TF1("900#circC",escape_probability,0.0,90.0,2);
  f_surface_Fr_900deg->SetParameters(depth,D_Fr[2]);
  TGraph *g_surface_Fr_900deg = new TGraph(f_surface_Fr_900deg);
  g_surface_Fr_900deg->SetMarkerColor(3);
  g_surface_Fr_900deg->SetLineColor(3);

  TF1 *f_surface_Fr_1200deg = new TF1("1200#circC",escape_probability,0.0,90.0,2);
  f_surface_Fr_1200deg->SetParameters(depth,D_Fr[3]);
  TGraph *g_surface_Fr_1200deg = new TGraph(f_surface_Fr_1200deg);
  g_surface_Fr_1200deg->SetMarkerColor(2);
  g_surface_Fr_1200deg->SetLineColor(2);

  TMultiGraph *mg_escape = new TMultiGraph();
  mg_escape->SetTitle("{}^{210}Fr Escape Probability from Depth 3 #mum in the Au (90 MeV range for E_{incident} = 100 MeV);Time (s);Probability");
  mg_escape->Add(g_surface_Fr_1200deg);
  mg_escape->Add(g_surface_Fr_900deg);
  mg_escape->Add(g_surface_Fr_600deg);
  mg_escape->Add(g_surface_Fr_300deg);

  mg_escape->Draw("ALP");
  c1->cd(1)->BuildLegend();

  c1->cd(2);

  // average escape time vs beam energy for various temperatures
  // using <t> = d^2/3D in the Fujioka1981 model

  double tAve_300deg[6];
  double tAve_600deg[6];
  double tAve_900deg[6];
  double tAve_1200deg[6];
  for (int i=0; i<6; ++i){
    double temperature_300deg[1] = {300.+273.};
    double temperature_600deg[1] = {600.+273.};
    double temperature_900deg[1] = {900.+273.};
    double temperature_1200deg[1] = {1200.+273.};
    double mass[1] = {211.};
    tAve_300deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i] / (3.0 * D_m(temperature_300deg,mass));
    tAve_600deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i] / (3.0 * D_m(temperature_600deg,mass));
    tAve_900deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i] / (3.0 * D_m(temperature_900deg,mass));
    tAve_1200deg[i] = depth_EXYZ70MeV[i]*depth_EXYZ70MeV[i] / (3.0 * D_m(temperature_1200deg,mass));
  }

  TGraph *g_tAve_300deg = new TGraph(6,inject_energy,tAve_300deg);
  g_tAve_300deg->SetTitle("300#circC");
  g_tAve_300deg->SetMarkerColor(9);
  g_tAve_300deg->SetLineColor(9);
  g_tAve_300deg->SetLineWidth(2);

  TGraph *g_tAve_600deg = new TGraph(6,inject_energy,tAve_600deg);
  g_tAve_600deg->SetTitle("600#circC");
  g_tAve_600deg->SetMarkerColor(4);
  g_tAve_600deg->SetLineColor(4);
  g_tAve_600deg->SetLineWidth(2);

  TGraph *g_tAve_900deg = new TGraph(6,inject_energy,tAve_900deg);
  g_tAve_900deg->SetTitle("900#circC");
  g_tAve_900deg->SetMarkerColor(3);
  g_tAve_900deg->SetLineColor(3);
  g_tAve_900deg->SetLineWidth(2);

  TGraph *g_tAve_1200deg = new TGraph(6,inject_energy,tAve_1200deg);
  g_tAve_1200deg->SetTitle("1200#circC");
  g_tAve_1200deg->SetMarkerColor(2);
  g_tAve_1200deg->SetLineColor(2);
  g_tAve_1200deg->SetLineWidth(2);

  TMultiGraph *mg_tAve = new TMultiGraph();
  mg_tAve->SetTitle("Average Escape Time of {}^{211}Fr from Au (from the 70 MeV range depth);Incident {}^{18}O Beam Energy (MeV);Escape Time <t> = #frac{d^{2}}{3D} (s)");

  mg_tAve->Add(g_tAve_300deg);
  mg_tAve->Add(g_tAve_600deg);
  mg_tAve->Add(g_tAve_900deg);
  mg_tAve->Add(g_tAve_1200deg);

  mg_tAve->Draw("AL");
  c1->cd(2)->BuildLegend();



  c1->cd(3);

  TF1 *f_D210fr = new TF1("D_{{}^{210}Fr}(T)",D_m,300.,2000.,1);
  f_D210fr->SetParameter(0,210.);
  TGraph *g_D210fr = new TGraph(f_D210fr);
  g_D210fr->SetTitle("Estimated Diffusion Coefficient of {}^{210}Fr in Au;Au Temperature (K);D_{{}^{210}Fr}(T) (cm^{2}/s)");
  g_D210fr->Draw("AL");



  c1->cd(4);

  double y_211Fr_300deg[6];
  double y_211Fr_600deg[6];
  double y_211Fr_900deg[6];
  double y_211Fr_1200deg[6];
  for (int i=0; i<6; ++i){
    double tau_211Fr = 3.10 * 60. / TMath::Log(2.); // seconds
    double temperature_300deg[1] = {300.+273.}; // K
    double temperature_600deg[1] = {600.+273.};
    double temperature_900deg[1] = {900.+273.};
    double temperature_1200deg[1] = {1200.+273.};
    double mass[1] = {211.};
    y_211Fr_300deg[i] = y_escape(tau_211Fr,D_m(temperature_300deg,mass),depth_EXYZ70MeV[i]);
    y_211Fr_600deg[i] = y_escape(tau_211Fr,D_m(temperature_600deg,mass),depth_EXYZ70MeV[i]);
    y_211Fr_900deg[i] = y_escape(tau_211Fr,D_m(temperature_900deg,mass),depth_EXYZ70MeV[i]);
    y_211Fr_1200deg[i] = y_escape(tau_211Fr,D_m(temperature_1200deg,mass),depth_EXYZ70MeV[i]);
  }
  TGraph *g_211frEscape_300deg = new TGraph(6,inject_energy,y_211Fr_300deg);
  g_211frEscape_300deg->SetTitle("300#circC");
  g_211frEscape_300deg->SetLineColor(9);
  g_211frEscape_300deg->SetLineWidth(2);

  TGraph *g_211frEscape_600deg = new TGraph(6,inject_energy,y_211Fr_600deg);
  g_211frEscape_600deg->SetTitle("600#circC");
  g_211frEscape_600deg->SetLineColor(4);
  g_211frEscape_600deg->SetLineWidth(2);

  TGraph *g_211frEscape_900deg = new TGraph(6,inject_energy,y_211Fr_900deg);
  g_211frEscape_900deg->SetTitle("900#circC");
  g_211frEscape_900deg->SetLineColor(3);
  g_211frEscape_900deg->SetLineWidth(2);

  TGraph *g_211frEscape_1200deg = new TGraph(6,inject_energy,y_211Fr_1200deg);
  g_211frEscape_1200deg->SetTitle("1200#circC");
  g_211frEscape_1200deg->SetLineColor(2);
  g_211frEscape_1200deg->SetLineWidth(2);

  TMultiGraph *mg_211frEscape = new TMultiGraph();
  mg_211frEscape->SetTitle("Number Ratios of {}^{211}Fr Escaping the Au Target before Decay (d = 70 MeV Range);Incident Beam Energy E_{incident} (MeV);y_{{}^{211}Fr}(E_{incident})");
  mg_211frEscape->Add(g_211frEscape_1200deg);
  mg_211frEscape->Add(g_211frEscape_900deg);
  mg_211frEscape->Add(g_211frEscape_600deg);
  mg_211frEscape->Add(g_211frEscape_300deg);
  mg_211frEscape->Draw("AL");

  c1->cd(4)->BuildLegend();


  rootapp.Run();

  c1->Update();
  c1->Modified();


  return 0;
}

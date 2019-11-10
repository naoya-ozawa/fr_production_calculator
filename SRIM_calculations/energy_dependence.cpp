#include <bits/stdc++.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TLatex.h>
#include <TRint.h>
using namespace std;

int main(int argc, char** argv){
	TRint rootapp("app",&argc,argv);
	TCanvas *c1 = new TCanvas();
	c1->Divide(2,1);

	c1->cd(1);

	TMultiGraph *mg_range = new TMultiGraph();
	mg_range->SetTitle("Energy/Stopping Range of {}^{18}O Ions in Solid Au vs Incident Beam Energy;Incident Energy E_{i} (MeV);Energy/Stopping Range X_{E_{threshold}} (#mum)");

	// 0.1 MeV (stopping) range
	double energy_stopping[7] = {70.,80.,90.,100.,110.,120.,130.};
	double energy_stopping_err[7] = {0.,0.,0.,0.,0.,0.,0.};
	double stopping_range[7] = {16.68,19.65,22.79,26.09,29.56,33.18,36.96};
	double stopping_range_err[7] = {0.44,0.42,0.52,0.55,0.53,0.68,0.78};

	TGraphErrors *g_stopping_range = new TGraphErrors(7,energy_stopping,stopping_range,energy_stopping_err,stopping_range_err);
//	g_stopping_range->SetTitle("Stopping Range of {}^{18}O Ions in Solid Au;Incident Energy E_{i} (MeV);Stopping Range X_{0} (#mum)");
	g_stopping_range->SetTitle("Stopping Range");
	g_stopping_range->SetMarkerColor(2);
	g_stopping_range->SetLineColor(2);
//	g_stopping_range->Draw("APL");
	mg_range->Add(g_stopping_range);

//	c1->cd(2);
	// 70 MeV range
	double energy_70mev[6] = {80.,90.,100.,110.,120.,130.};
	double energy_70mev_err[6] = {0.,0.,0.,0.,0.,0.};
	double e70mev_range[6] = {2.72,5.86,9.18,12.65,16.28,20.07};
	double e70mev_range_err[6] = {0.05,0.08,0.13,0.12,0.19,0.29};

	TGraphErrors *g_70mev_range = new TGraphErrors(6,energy_70mev,e70mev_range,energy_70mev_err,e70mev_range_err);
//	g_70mev_range->SetTitle("70 MeV Range of {}^{18}O Ions in Solid Au;Incident Energy E_{i} (MeV);Stopping Range X_{70 MeV} (#mum)");
	g_70mev_range->SetTitle("E_{threshold} = 70 MeV");
	g_70mev_range->SetMarkerColor(3);
	g_70mev_range->SetLineColor(3);
//	g_70mev_range->Draw("APL");
	mg_range->Add(g_70mev_range);

//	TF1 *f_70mev_range = new TF1("f_70mev_range","[0] + [1]*TMath::Exp([2]*x)",75.,135.);
//	f_70mev_range->SetParameters(-20.0,0.1,0.1);
//	g_70mev_range->Fit(f_70mev_range);


//	c1->cd(3);
	// 80 MeV range
	double energy_80mev[5] = {90.,100.,110.,120.,130.};
	double energy_80mev_err[5] = {0.,0.,0.,0.,0.};
	double e80mev_range[5] = {2.87,6.19,9.66,13.29,17.08};
	double e80mev_range_err[5] = {0.06,0.09,0.10,0.16,0.24};

	TGraphErrors *g_80mev_range = new TGraphErrors(5,energy_80mev,e80mev_range,energy_80mev_err,e80mev_range_err);
//	g_80mev_range->SetTitle("80 MeV Range of {}^{18}O Ions in Solid Au;Incident Energy E_{i} (MeV);Stopping Range X_{80 MeV} (#mum)");
	g_80mev_range->SetTitle("E_{threshold} = 80 MeV");
	g_80mev_range->SetMarkerColor(4);
	g_80mev_range->SetLineColor(4);
//	g_80mev_range->Draw("APL");
	mg_range->Add(g_80mev_range);


//	c1->cd(4);
	// 90 MeV range
	double energy_90mev[4] = {100.,110.,120.,130.};
	double energy_90mev_err[4] = {0.,0.,0.,0.};
	double e90mev_range[4] = {3.03,6.50,10.13,13.92};
	double e90mev_range_err[4] = {0.06,0.08,0.13,0.20};

	TGraphErrors *g_90mev_range = new TGraphErrors(4,energy_90mev,e90mev_range,energy_90mev_err,e90mev_range_err);
//	g_90mev_range->SetTitle("90 MeV Range of {}^{18}O Ions in Solid Au;Incident Energy E_{i} (MeV);Stopping Range X_{90 MeV} (#mum)");
	g_90mev_range->SetTitle("E_{threshold} = 90 MeV");
	g_90mev_range->SetMarkerColor(7);
	g_90mev_range->SetLineColor(7);
//	g_90mev_range->Draw("APL");
	mg_range->Add(g_90mev_range);


//	c1->cd(5);
	// 100 MeV range
	double energy_100mev[3] = {110.,120.,130.};
	double energy_100mev_err[3] = {0.,0.,0.};
	double e100mev_range[3] = {3.17,6.81,10.59};
	double e100mev_range_err[3] = {0.06,0.11,0.15};

	TGraphErrors *g_100mev_range = new TGraphErrors(3,energy_100mev,e100mev_range,energy_100mev_err,e100mev_range_err);
//	g_100mev_range->SetTitle("100 MeV Range of {}^{18}O Ions in Solid Au;Incident Energy E_{i} (MeV);Stopping Range X_{100 MeV} (#mum)");
	g_100mev_range->SetTitle("E_{threshold} = 100 MeV");
	g_100mev_range->SetMarkerColor(6);
	g_100mev_range->SetLineColor(6);
//	g_100mev_range->Draw("APL");
	mg_range->Add(g_100mev_range);

	mg_range->Draw("APL");
	c1->cd(1)->BuildLegend();
	c1->cd(1)->SetGrid(1,1);

	c1->cd(2);
	// X_{E_th} vs E_th plot for E_i = 130 MeV
	double energy_threshold[5] = {0.,70.,80.,90.,100.};
	double energy_threshold_err[5] = {0.,0.,0.,0.,0.};
	double range_threshold[5] = {stopping_range[6],e70mev_range[5],e80mev_range[4],e90mev_range[3],e100mev_range[2]};
	double range_threshold_err[5] = {stopping_range_err[6],e70mev_range_err[5],e80mev_range_err[4],e90mev_range_err[3],e100mev_range_err[2]};

	TGraphErrors *g_threshold_dependence = new TGraphErrors(5,energy_threshold,range_threshold,energy_threshold_err,range_threshold_err);
	g_threshold_dependence->SetTitle("Threshold Energy Dependence on Energy/Stopping Range for E_{i} = 130 MeV;Threshold Energy E_{threshold} (MeV);Energy/Stopping Range X_{E_{threshold}} (#mum)");
	g_threshold_dependence->Draw("APL");
	c1->cd(2)->SetGrid(1,1);

//	TF1 *f_threshold_dependence = new TF1("f_threshold_dependence","[0]*TMath::Log([1]+[2]*x)",-1.0,101.);
//	f_threshold_dependence->SetParameters(1.0,1.0,1.0);
//	g_threshold_dependence->Fit(f_threshold_dependence);
//	double C_2 = f_threshold_dependence->GetParameter(1);
//	double C_2_err = f_threshold_dependence->GetParError(1);
//	double C_3 = f_threshold_dependence->GetParameter(2);
//	double C_3_err = f_threshold_dependence->GetParError(2);

//	c1->cd(2);
//	TF1 *f_70mev_range = new TF1("f_70mev_range","[0] + [3]*TMath::Exp([1]+[2]*x)");
//	f_70mev_range->SetParameters(1.0,1.0,1.0,1.0);
//	f_70mev_range->FixParameter(0,0.0);
//	f_70mev_range->FixParameter(3,TMath::Log(C_2+C_3*70.));

//	g_70mev_range->Fit(f_70mev_range);

  rootapp.Run();
  c1->Update();
  c1->Modified();

  return 0;
}

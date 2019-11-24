void fr_production_estimation(){

  // Input for CNS Case
  double beam_current = 3.*TMath::Power(10.,-6); // eA
  double beam_flux = (beam_current/(1.6e-19))/6.0; // For 6+ ions
  double ext_eff = 0.1; // extraction efficiency of the ion source
  double geo_eff = 0.001; // detection efficiency of SSD

  double br_208fr = 0.90;
  double br_209fr = 0.89;
  double br_210fr = 0.74;
  double br_211fr = 0.8;

  double energy[10] = {82.,86.,90.,94.,98.,102.,106.,110.,114.,118.}; // MeV incident
  double fr208[10] = {0.,0.,0.,1.8,2.1e+01,3.1e+03,4.7e+04,2.3e+05,5.9e+05,1.8e+06}; // Hz for j = 1e+12 particles/s
  double fr209[10] = {2.3e+01,7.6e+01,5.7e+03,1.2e+05,6.0e+05,1.5e+06,2.4e+06,2.9e+06,3.2e+06,3.2e+06};
  double fr210[10] = {2.2e+04,3.8e+05,1.5e+06,2.9e+06,3.9e+06,4.3e+06,4.4e+06,4.4e+06,4.4e+06,4.4e+06};
  double fr211[10] = {4.3e+05,8.9e+05,1.1e+06,1.2e+06,1.2e+06,1.2e+06,1.2e+06,1.2e+06,1.2e+06,1.2e+06};

  double nfr208[10];
  double nfr209[10];
  double nfr210[10];
  double nfr211[10];
  for (int i=0; i<10; ++i){
    nfr208[i] = fr208[i] / TMath::Power(10.,12);
    nfr209[i] = fr209[i] / TMath::Power(10.,12);
    nfr210[i] = fr210[i] / TMath::Power(10.,12);
    nfr211[i] = fr211[i] / TMath::Power(10.,12);
  }

  TCanvas *c1 = new TCanvas();
  c1->Divide(2,1);

  c1->cd(1);

  TGraphErrors *ge_fr208 = new TGraphErrors(10,energy,nfr208);
  ge_fr208->SetTitle("{}^{208}Fr");
  ge_fr208->SetLineColor(3);
  TGraphErrors *ge_fr209 = new TGraphErrors(10,energy,nfr209);
  ge_fr209->SetTitle("{}^{209}Fr");
  ge_fr209->SetLineColor(4);
  TGraphErrors *ge_fr210 = new TGraphErrors(10,energy,nfr210);
  ge_fr210->SetTitle("{}^{210}Fr");
  ge_fr210->SetLineColor(2);
  TGraphErrors *ge_fr211 = new TGraphErrors(10,energy,nfr211);
  ge_fr211->SetTitle("{}^{211}Fr");
  ge_fr211->SetLineColor(5);

  TMultiGraph *mg_fr_production = new TMultiGraph();
  mg_fr_production->SetTitle("Fr production ratio (from calculation in Stancari2006);Incident Beam Energy (MeV);Normalized Production Rate P/j");
  mg_fr_production->Add(ge_fr208);
  mg_fr_production->Add(ge_fr209);
  mg_fr_production->Add(ge_fr210);
  mg_fr_production->Add(ge_fr211);

  mg_fr_production->Draw("ALP");
  c1->cd(1)->BuildLegend();


  c1->cd(2);

  double detect_fr208[10];
  double detect_fr209[10];
  double detect_fr210[10];
  double detect_fr211[10];
  for (int i=0; i<10; ++i){
    detect_fr208[i] = nfr208[i]*beam_flux*br_208fr*ext_eff*geo_eff;
    detect_fr209[i] = nfr209[i]*beam_flux*br_209fr*ext_eff*geo_eff;
    detect_fr210[i] = nfr210[i]*beam_flux*br_210fr*ext_eff*geo_eff;
    detect_fr211[i] = nfr211[i]*beam_flux*br_211fr*ext_eff*geo_eff;
  }

  TGraphErrors *ge_detect_fr208 = new TGraphErrors(10,energy,detect_fr208);
  ge_detect_fr208->SetTitle("{}^{208}Fr / E_{#alpha}=6.64 MeV / #tau_{1/2}=59 s");
  ge_detect_fr208->SetLineColor(3);
  TGraphErrors *ge_detect_fr209 = new TGraphErrors(10,energy,detect_fr209);
  ge_detect_fr209->SetTitle("{}^{209}Fr / E_{#alpha}=6.65 MeV / #tau_{1/2}=50 s");
  ge_detect_fr209->SetLineColor(4);
  TGraphErrors *ge_detect_fr210 = new TGraphErrors(10,energy,detect_fr210);
  ge_detect_fr210->SetTitle("{}^{210}Fr / E_{#alpha}=6.54 MeV / #tau_{1/2}=191 s");
  ge_detect_fr210->SetLineColor(2);
  TGraphErrors *ge_detect_fr211 = new TGraphErrors(10,energy,detect_fr211);
  ge_detect_fr211->SetTitle("{}^{211}Fr / E_{#alpha}=6.53 MeV / #tau_{1/2}=186 s");
  ge_detect_fr211->SetLineColor(5);

  TMultiGraph *mg_detection = new TMultiGraph();
  mg_detection->SetTitle(Form("Estimated #alpha detection rate at SSD (for %g e#muA beam);Incident {}^{18}O Beam Energy (MeV);Detection Rate (Hz)",beam_current*TMath::Power(10.,6)));
  mg_detection->Add(ge_detect_fr208);
  mg_detection->Add(ge_detect_fr209);
  mg_detection->Add(ge_detect_fr210);
  mg_detection->Add(ge_detect_fr211);

  mg_detection->Draw("ALP");
  c1->cd(2)->BuildLegend();

}

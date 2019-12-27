void expdata_checker(){
  TCanvas *c1 = new TCanvas();

  const char* filename = "./EvaporationResidueCS_18O197Au_alpha0.013_gamma0.1.txt";

//  TGraphErrors *ge_1n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ge_2n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ge_3n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ge_4n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ge_5n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ge_6n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
//  TGraphErrors *ge_8n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
//  TGraphErrors *ge_9n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  int n_samples = ge_2n->GetN();

  // Experimental data from Corradi2005
  double energy[16] = {74.9,79.9,84.9,87.4,89.9,89.9,92.4,94.9,97.4,99.9,104.9,109.9,114.9,119.9,124.9,129.9}; // MeV(lab)
  double cs_4n5n[16] = {1.1,54.0,198.8,206.2,223.9,218.3,160.4,115.1,66.4,36.3,12.3,0.,0.,0.,0.,0.}; // mb
  double cs_6n7n[16] = {0.,0.,0.,0.,0.,0.,0.,23.9,55.1,73.3,106.3,96.1,71.7,41.2,24.5,10.4}; // mb
  double cs_8n[16] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,5.3,16.0,19.9}; // mb
  double cs_4n5n_err[16], cs_6n7n_err[16], cs_8n_err[16];
  double energy_err[16] = {0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};
  for (int i=0; i<16; ++i){
    cs_4n5n_err[i] = cs_4n5n[i]*0.2;
    cs_6n7n_err[i] = cs_6n7n[i]*0.2;
    cs_8n_err[i] = cs_8n[i]*0.2;
  }
  TGraphErrors *ge_4n5n_exp = new TGraphErrors(16,energy,cs_4n5n,energy_err,cs_4n5n_err);
  ge_4n5n_exp->SetTitle("Expt. data for 4n & 5n");
  ge_4n5n_exp->SetMarkerStyle(8);
  ge_4n5n_exp->SetMarkerColor(2);
  TGraphErrors *ge_6n7n_exp = new TGraphErrors(16,energy,cs_6n7n,energy_err,cs_6n7n_err);
  ge_6n7n_exp->SetTitle("Expt. data for 6n & 7n");
  ge_6n7n_exp->SetMarkerStyle(21);
  ge_6n7n_exp->SetMarkerColor(3);
  TGraphErrors *ge_8n_exp = new TGraphErrors(16,energy,cs_8n,energy_err,cs_8n_err);
  ge_8n_exp->SetTitle("Expt. data for 8n");
  ge_8n_exp->SetMarkerStyle(23);
  ge_8n_exp->SetMarkerColor(4);

  TGraphErrors *ge_4n5n_calc = new TGraphErrors(n_samples);
  ge_4n5n_calc->SetTitle("NRV Calc. for 4n & 5n");
  ge_4n5n_calc->SetLineColor(2);
  ge_4n5n_calc->SetLineWidth(2);
//  TGraphErrors *ge_6n7n_calc = new TGraphErrors(n_samples);
//  ge_6n7n_calc->SetTitle("NRV Calc. for 6n & 7n");
//  ge_6n7n_calc->SetLineColor(3);
//  ge_6n7n_calc->SetLineWidth(2);
//  TGraphErrors *ge_8n_calc = new TGraphErrors(n_samples);
//  ge_8n_calc->SetTitle("NRV Calc. for 8n");
//  ge_8n_calc->SetLineColor(4);
//  ge_8n_calc->SetLineWidth(2);

  // Convert CM frame to LAB frame and combine calculations
  for (int i=0; i<n_samples; ++i){
    double mAu = 197.0; // u
    double mO = 18.0; // u
//    double e_cm = ge_2n->GetX()[i] + mO; // MeV O (KE + mass)
//    double p_cm = TMath::Sqrt(e_cm*e_cm - mO*mO);
//    double eAucm = TMath::Sqrt(mAu*mAu + p_cm*p_cm);
//    double betaA = p_cm/eAucm;
//    double gammaA = eAucm/mAu;
//    double e_lab = gammaA*e_cm + betaA*gammaA*p_cm - mO; // MeV O (KE)
    double e_lab = ge_2n->GetX()[i] * (1. + (mO/mAu));
    ge_4n5n_calc->GetX()[i] = e_lab;
    ge_4n5n_calc->GetY()[i] = ge_4n->GetY()[i]+ge_5n->GetY()[i];
//    ge_6n7n_calc->GetX()[i] = e_lab;
//    ge_6n7n_calc->GetY()[i] = ge_6n->GetY()[i]+ge_7n->GetY()[i];
//    ge_8n_calc->GetX()[i] = e_lab;
//    ge_8n_calc->GetY()[i] = ge_8n->GetY()[i];
  }

  TMultiGraph *mg = new TMultiGraph();
  mg->SetTitle("Fusion-Evaporation Cross Sections for {}^{197}Au ({}^{18}O, xn) {}^{215-x}Fr;E_{lab} of {}^{18}O Beam (MeV);Cross Section (mb)");
  mg->Add(ge_4n5n_exp);
  mg->Add(ge_4n5n_calc);
  mg->Add(ge_6n7n_exp);
//  mg->Add(ge_6n7n_calc);
  mg->Add(ge_8n_exp);
//  mg->Add(ge_8n_calc);
  mg->Draw("APL");

//  ge_4n5n_exp->Draw("P");
//  ge_6n7n_exp->Draw("P");
//  ge_8n_exp->Draw("P");

//  ge_4n5n_calc->Draw("PL");
//  ge_6n7n_calc->Draw("PL");
//  ge_8n_calc->Draw("PL");

  c1->BuildLegend();


}

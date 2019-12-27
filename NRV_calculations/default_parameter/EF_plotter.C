void EF_plotter(){
  const char* filename = "./EvaporationResidueCS_30Si181Ta.dat";
  TMultiGraph *mg = new TMultiGraph();
  mg->SetTitle("Calculated Excitation Function for {}^{181}Ta ({}^{30}Si, xn) {}^{211-x}Fr;Beam Energy E_{lab} (MeV);Evaporation Residue Cross Section #sigma_{ER} (mb)");

  TGraphErrors *ef_0n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ef_1n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ef_3n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ef_2n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ef_5n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ef_4n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
  TGraphErrors *ef_6n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
//  TGraphErrors *ef_7n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
//  TGraphErrors *ef_8n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
//  TGraphErrors *ef_3n = new TGraphErrors(filename,"%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");

  ef_0n->SetTitle("0n");
  ef_0n->SetLineColor(9);
  ef_0n->SetLineWidth(2);
  mg->Add(ef_0n);
  ef_1n->SetTitle("1n");
  ef_1n->SetLineColor(1);
  ef_1n->SetLineWidth(2);
  mg->Add(ef_1n);
  ef_2n->SetTitle("2n");
  ef_2n->SetLineColor(2);
  ef_2n->SetLineWidth(2);
  mg->Add(ef_2n);
  ef_3n->SetTitle("3n");
  ef_3n->SetLineColor(3);
  ef_3n->SetLineWidth(2);
  mg->Add(ef_3n);
  ef_4n->SetTitle("4n");
  ef_4n->SetLineColor(4);
  ef_4n->SetLineWidth(2);
  mg->Add(ef_4n);
  ef_5n->SetTitle("5n");
  ef_5n->SetLineColor(5);
  ef_5n->SetLineWidth(2);
  mg->Add(ef_5n);
  ef_6n->SetTitle("6n");
  ef_6n->SetLineColor(6);
  ef_6n->SetLineWidth(2);
  mg->Add(ef_6n);
//  ef_7n->SetTitle("7n");
//  ef_7n->SetLineColor(7);
//  ef_7n->SetLineWidth(2);
//  mg->Add(ef_7n);
//  ef_8n->SetTitle("8n");
//  ef_8n->SetLineColor(8);
//  ef_8n->SetLineWidth(2);
//  mg->Add(ef_8n);
//  ef_9n->SetTitle("9n");
//  ef_9n->SetLineColor(9);
//  ef_9n->SetLineWidth(2);
//  mg->Add(ef_9n);
//  ef_10n->SetTitle("10n");
//  ef_10n->SetLineColor(2);
//  ef_10n->SetLineWidth(2);
//  mg->Add(ef_10n);

  mg->Draw("ACP");
  gPad->BuildLegend();
  gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();

}

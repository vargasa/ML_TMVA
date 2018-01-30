TCanvas *GenData(){

  Int_t n = 2e6;

  TTree *treebckg = new TTree("treebckg","Background"); // Background Tree
  TTree *treesig = new TTree("treesig","Signal"); // Signal Tree
    
  Float_t x_bckg = .0; 
  Float_t x_sig = .0;
  Float_t x = .0;;
  
  TBranch *bckg = treebckg->Branch("bckg", &x_bckg);
  TBranch *sig = treesig->Branch("sig",&x_sig);
  TBranch *bx = treesig->Branch("bx",&x);

  // [1] -> Sigma^2 Variance
  // [0] -> Mu Median

  TF1 *fb = new TF1("f1","(1/sqrt(2*pi*[1]^2))*exp((-(x-[0])^2)/(2*[1]^2))",0,100);
  fb->SetParameter(0,50.);
  fb->SetParameter(1,50);
  
  TF1 *fs = new TF1("f2","(1/sqrt(2*pi*[1]^2))*exp((-(x-[0])^2)/(2*[1]^2))",0,100);
  fs->SetParameter(0,30);
  fs->SetParameter(1,1.5);

  TH1F *hb = new TH1F("hb","Background",100,0,100);
  TH1F *hs = new TH1F("hs","Signal",100,0,100);
  TH1F *hbs = new TH1F("hbs","Background And Signal",100,0,100);
 
  for (Int_t i = 0; i < n; i++) {
    x_bckg = fb->GetRandom();
    x_sig = fs->GetRandom();
    x = x_bckg + x_sig;
    
    bckg->Fill();
    sig->Fill();
    bx->Fill();

    hb->Fill(x_bckg);
    hs->Fill(x_sig);
    hbs->Fill(x);
    
  }

  TCanvas *cc = new TCanvas("cc","Complete set");
  cc->Divide(1,3);
  cc->cd(1);
  hb->Draw("HIST");
  cc->cd(2);
  hs->Draw("HIST");
  cc->cd(3);
  hbs->Draw("HIST");
  cc->cd(4);
  //tree->Draw("bx","1","HIST");

  TFile *fOut = new TFile("DataSet.root","RECREATE");
  treebckg->Write();
  treesig->Write();
  fOut->Close();

  return cc;
 

}

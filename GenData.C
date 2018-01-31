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

  TF1 *fb = new TF1("fb","(1/sqrt(2*pi*[1]^2))*exp((-(x-[0])^2)/(2*[1]^2))",0,100);
  fb->SetParameter(0,50.);
  fb->SetParameter(1,50);
  
  TF1 *fs = new TF1("fs","(1/sqrt(2*pi*[1]^2))*exp((-(x-[0])^2)/(2*[1]^2))",0,100);
  fs->SetParameter(0,30);
  fs->SetParameter(1,1.5);

  TH1F *hb = new TH1F("hb","Background",100,0,100);
  TH1F *hs = new TH1F("hs","Signal",100,0,100);
  TH1F *hbs = new TH1F("hbs","Background And Signal",100,0,100);
 
  for (Int_t i = 0; i < n; i++) {
    x_bckg = fb->GetRandom();
    x_sig = fs->GetRandom();
    x = x_bckg + x_sig;
    
    treebckg->Fill();
    treesig->Fill();

    hb->Fill(x_bckg);
    hs->Fill(x_sig);
    hbs->Fill(x);
    
  }

  TCanvas *cc = new TCanvas("cc","Complete set");
  cc->Divide(2,3);
  cc->cd(1);
  hb->Draw("HIST");
  cc->cd(2);
  hs->Draw("HIST");
  cc->cd(3);
  hbs->Draw("HIST");
  //tree->Draw("bx","1","HIST");

  // Correlation
  // Is it possible to train a NN to return sin(x) values?

  Double_t sinx;
  Double_t x2;
  
  TTree *treesinx = new TTree("sin","Sin Function");
  TBranch *bsinx = treesinx->Branch("sinx",&sinx);
  TBranch *bx2 = treesinx->Branch("x2",&x2);

  TH1F *hsin = new TH1F("hsin","Sin",10,-1.,1. );
  TH1F *hx = new TH1F("hx","x",100,-2*TMath::Pi(), 2*TMath::Pi());

  TRandom3 *rnd = new TRandom3();
  
  for (Int_t i = 0; i < n; i++){
    x2 = rnd->Uniform(-2*TMath::Pi(),2*TMath::Pi());
    sinx = TMath::Sin(x2);
    hsin->Fill(sinx);
    hx->Fill(x2);
    treesinx->Fill();
  }

  cc->cd(4); 
  hx->Draw("HIST");
  cc->cd(5);
  hsin->Draw("HIST");

  TFile *fOut = new TFile("DataSet.root","RECREATE");
  treesinx->Write();
  treebckg->Write();
  treesig->Write();
  fOut->Close();

  return cc;
 

}

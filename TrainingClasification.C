


void TrainingClasification(){

  Int_t j = 1e5; // # of Events used for Training phase

  TFile *fIn = new TFile("DataSet.root","READ");

  // Where control and performance histograms are stored
  TFile *fOut = new TFile("SigBackPerf.root","RECREATE");

  TMVA::Factory *fct = new TMVA::Factory("SigBackClas",fOut,"");

  TMVA::DataLoader *dlo = new TMVA::DataLoader("dataset");

  TTree *tb = (TTree*)fIn->Get("treebckg");
  //tb->Print();

  TTree *ts = (TTree*)fIn->Get("treesig");
  //ts->Print();

  // Subset for training
  TTree *treebckg = tb->CloneTree(j);  
  TTree *treesig = ts->CloneTree(j);

  dlo->AddSignalTree(treesig,1.0);
  dlo->AddBackgroundTree(treebckg,1.0);

  dlo->PrepareTrainingAndTestTree("","",
				 "SplitMode=Random:NormMode=NumEvents:!V");
  

  fct->BookMethod(dlo, TMVA::Types::kCuts, "Cuts",
		       "!H:!V:FitMethod=MC:EffSel:SampleSize=100000:VarProp=FSmart" );
  
  fct->TrainAllMethods();
  fct->TestAllMethods();
  fct->EvaluateAllMethods();
  
  fOut->Close();

  
}

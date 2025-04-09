#include <vector>
#include <math.h>
#include <iostream>

#include "TGraph.h"
#include "TMultiGraph.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"

//Originally authored by the ROOT Team: https://root.cern/doc/master/hvector_8C.html

typedef struct {
  std::vector<float> vf;
  std::vector<bool> vb;
} multidata;

void write(Float_t &comp, int N=1e6, int autoflush=-30000000, int vecsize=50, int bsize=4) // that is the default autoflush value 
{
 
  TFile *f = TFile::Open(Form("hvector_%d_%d_%d_%d.root",N,autoflush,vecsize,bsize), "RECREATE");
  float percentComplete = 0.0f; 
  if (!f) { return; }
   
  std::vector<int> vi;
  multidata vecs;

  // Create a TTree
  TTree *t = new TTree("tvec","Tree with vectors");
  t->SetAutoFlush(autoflush);
  t->Branch("vecs",&vecs,"vf/F:vb/O");
  t->Branch("vi",&vi);
  t->SetBasketSize("*", bsize*1000);  

  gRandom->SetSeed();
  for (Int_t i = 0; i < N; i++) {

    // Monitor Progress
    //if(i%10000==0) {
    //  percentComplete =((float)i/N)*100.f;
    //  std::cerr << "\rLOOP [write()]: " << percentComplete << "\% Complete" <<  std::flush;
    //}
    
    
    Int_t npx = (Int_t)(gRandom->Rndm(1)*vecsize);
    
    vecs.vf.clear();
    vecs.vb.clear();
    vi.clear();            
    
    for (Int_t j = 0; j < npx; ++j) {
 
      Float_t pf;
      Float_t pb;
      Int_t integer;
      pf = gRandom->Rndm(1);
      pb = gRandom->Integer(2);
      integer = (int)(gRandom->Rndm(1)*npx + npx);
      vecs.vf.emplace_back(pf);
      vecs.vb.emplace_back(pb);
      vi.emplace_back(integer);
    }
    t->Fill();
  }

  comp = (t->GetBranch("vecs")->GetTotBytes() + 0.00001)/(t->GetBranch("vecs")->GetZipBytes());
  std::cerr << std::endl;
  t->Print();
  std::cerr << std::endl;
  f->Write();
  delete f;
}
 
 
 
int main()
{
   int N = 1e6; // match this with N in readROOTFile.cpp
   int vecsize=50;
   Float_t realtime;
   Float_t cputime;
   std::vector<int> flushes;
   flushes.emplace_back(0);
   int bSize[] = {4, 8, 16, 32, 64, 128, 256, 512, 1024};
   const char* form;
   // Timed write()
   TGraph *cxg = new TGraph();
   TGraph *rt = new TGraph();
   rt->SetTitle("Real Time");
   rt->SetLineColor(kRed);
   TGraph *ct = new TGraph();
   ct->SetTitle("CPU Time");
   ct->SetLineColor(kBlue);
   cxg->SetTitle("Compression vs. Basket Size for variable data;Log2 of Basket Size (kB);Compression Factor");
   Float_t comp;
   for (auto flush : flushes) {
     for (auto basket : bSize) {
       std::cerr << "\nFlush = " << flush << " and N = " << N << " and vec size = " << vecsize << " and basket size = " << basket << std::endl;
       gBenchmark = new TBenchmark();
       form =  Form("wr_hvec_N%d_F%d_Sz%d_Bs%d",N,flush,vecsize,basket);
       gBenchmark->Start(form);
       write(comp, N, flush, vecsize, basket);
       cxg->AddPoint(log2(basket), comp);
       std::cout << std::endl; 
       gBenchmark->Stop(form);
       gBenchmark->Summary(realtime,cputime);
       rt->AddPoint(log2(basket), realtime);
       ct->AddPoint(log2(basket), cputime);
       delete gBenchmark;
     }
   }

   TMultiGraph *mg = new TMultiGraph();
   mg->SetTitle("Time vs. Basket Size for variable data;Log2 of Basket Size (kB);Time (s)");
   mg->Add(rt, "AL*");
   mg->Add(ct, "AL*");
   TCanvas *c1 = new TCanvas("c1", "write",200,10,700,500);
   cxg->Draw("AL*");
   c1->SaveAs("pdfs/graph_compression_variable.pdf");

   c1->Clear();
   mg->Draw("AL*");
   c1->BuildLegend();
   c1->SaveAs("pdfs/graph_time_variable.pdf");

   return 0;
}

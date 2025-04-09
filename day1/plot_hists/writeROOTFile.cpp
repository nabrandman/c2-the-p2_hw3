#include <vector>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"

//Originally authored by the ROOT Team: https://root.cern/doc/master/hvector_8C.html


void write(int N=1e6, int autoflush=-30000000, int vecsize=50) // that is the default autoflush value 
{
 
   TFile *f = TFile::Open(Form("hvector_%d_%d_%d.root",N,autoflush,vecsize), "RECREATE");
   float percentComplete = 0.0f; 
   if (!f) { return; }

   TH1F *hpt = new TH1F("hpt", "Pt distribution",100,0,5);
   TH1F *hpx = new TH1F("hpx", "Px distribution",100,-5,5);
   TH1F *hpy = new TH1F("hpy", "Py distribution",100,-5,5);
   TH1F *hpz = new TH1F("hpz", "Pz distribution",100,0,1);
   hpt->SetFillColor(48);
   hpx->SetFillColor(48);
   hpy->SetFillColor(48);
   hpz->SetFillColor(48);
   
   std::vector<float> vpx;
   std::vector<float> vpy;
   std::vector<float> vpz;
   std::vector<float> vpt;   
   std::vector<int> vint;

   // Create a TTree
   TTree *t = new TTree("tvec","Tree with vectors");
   t->SetAutoFlush(autoflush);
   t->Branch("vpx",&vpx);
   t->Branch("vpy",&vpy);
   t->Branch("vpz",&vpz);   
   t->Branch("vpt",&vpt);
   t->Branch("vint",&vint);   

   gRandom->SetSeed();
   for (Int_t i = 0; i < N; i++) {

      // Monitor Progress
      //if(i%10000==0) {
     //   percentComplete =((float)i/N)*100.f;
     //	   std::cerr << "\rLOOP [write()]: " << percentComplete << "\% Complete" <<  std::flush;
     //}


      Int_t npx = (Int_t)(gRandom->Rndm(1)*vecsize);
 
      vpx.clear();
      vpy.clear();
      vpz.clear();
      vpt.clear();
      vint.clear();            
 
      for (Int_t j = 0; j < npx; ++j) {
 
         Float_t px,py,pz,pt;
         Int_t integer;
         gRandom->Rannor(px,py);
         pt = sqrt(px*px + py*py);
         pz = gRandom->Rndm(1);
         integer = (int)(gRandom->Rndm(1)*npx + npx);

	 hpt->Fill(pt);
	 hpx->Fill(px);
	 hpy->Fill(py);
	 hpz->Fill(pz);
	 
         vpx.emplace_back(px);
         vpy.emplace_back(py);
         vpz.emplace_back(pz);
         
         /// this gets pushed back twice on purpose
         vpt.emplace_back(pt);
         vpt.emplace_back(pt);         
         vint.emplace_back(integer);         
 
      }
      t->Fill();
   }
   
   t->Print();
   f->Write();
   TCanvas *c1 = new TCanvas("c1", "write",200,10,700,500);
   hpt->Draw();
   c1->SaveAs(Form("pdfs/write_pt_%d_%d_%d.pdf",N,autoflush,vecsize));

   c1->Clear();
   hpx->Draw();
   c1->SaveAs(Form("pdfs/write_px_%d_%d_%d.pdf",N,autoflush,vecsize));

   c1->Clear();
   hpy->Draw();
   c1->SaveAs(Form("pdfs/write_py_%d_%d_%d.pdf",N,autoflush,vecsize));

   c1->Clear();
   hpz->Draw();
   c1->SaveAs(Form("pdfs/write_pz_%d_%d_%d.pdf",N,autoflush,vecsize));
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
   flushes.emplace_back(-30000000);
   flushes.emplace_back(-1000000);
   flushes.emplace_back(1e4);
   flushes.emplace_back(1e2);   
   const char* form;
   // Timed write()
   for (auto flush : flushes) {
      std::cerr << "Flush = " << flush << " and N = " << N << " and vec size = " << vecsize << std::endl;
      gBenchmark = new TBenchmark();
      form =  Form("wr_hvec_N%d_F%d_Sz%d",N,flush,vecsize);
      gBenchmark->Start(form);
      write(N, flush); 
      std::cout << std::endl; 
      gBenchmark->Stop(form);
      gBenchmark->Summary(realtime, cputime);
      delete gBenchmark;
   }

   return 0;
}

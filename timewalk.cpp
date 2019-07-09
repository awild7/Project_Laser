
#include<iostream>
#include<cmath>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"

using namespace std;

int main(int argc,char ** argv){
  if ( argc !=3 ){
    cout <<"Error wrong amount of arguments used\n"
	 <<"provide path to input and to output\n";
    
  }
  cout <<"hello world!\n";
  cout <<argc<< " "<<argv<< "\n";
  
  //getting the file
  TFile * inputFile = new TFile(argv[1]);
  TFile * outputFile = new TFile(argv[2], "RECREATE");
  cout<<"File has been opened from: "<<argv[1]<<"\n";
  
  //reading the file
  TH2D *h = (TH2D*) inputFile->Get("TvA_1");
   cout << "made projection"<<"\n";
  //making ALOT of gaussians out of the file and graphing
   //  double integral=h->Integral();
   // cout << integral <<"\n";
  TAxis *l = (TAxis *) h->GetXaxis();
  cout <<h->GetXaxis()->GetNbins()<<"\n";
  int binum= l->FindBin(550);
  int binum2= l->FindBin(1650);
  cout <<"first bin#:" <<binum<<"\n";
   cout <<"last bin#:" <<binum2<<"\n";

   TGraph *gr1 = new TGraph();
   gr1->SetName("mean_graph");
   for (int binn=0;binn<42;binn=binn+1){
     int binn2= binn+1;
     TH1D*g=(TH1D*)h->ProjectionY("",binn,binn2,"[cutg1],[cutg2]");
     outputFile->cd();
     g->Write();
     g->Draw();
     double d=g->GetMean();
     gr1->SetPoint(gr1->GetN(),l->GetBinCenter(binn),d);
   }
   gr1->Write();
  return 1;
  
}


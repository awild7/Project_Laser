
#include<iostream>
#include<cmath>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TH2.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TAxis.h"

using namespace std;

int main(int argc,char ** argv){
  if ( argc !=3 ){
    cout <<"Error wrong amount of arguments used\n"
	 <<"provide path to input and to output\n";
    
  }

  //getting the file
  TFile * inputFile = new TFile(argv[1]);
  TFile * outputFile = new TFile(argv[2], "RECREATE");
  cout<<"File has been opened from: "<<argv[1]<<"\n";
  cout<< inputFile->GetListOfKeys()->GetEntries()<<"\n";
  //reading the file
  for(int i = 0; i < inputFile->GetListOfKeys()->GetEntries(); i++){
    if(inputFile->Get(inputFile->GetListOfKeys()->At(i)->GetName())->InheritsFrom(TH2D::Class())){
    cout<<inputFile->GetListOfKeys()->At(i)->GetName()<<"\n";
    TH2D * onehist = (TH2D*)inputFile->Get(inputFile->GetListOfKeys()->At(i)->GetName());
    }
  }
 
  TH2D * TvAhist = (TH2D*) inputFile->Get("TvA_3");
  TAxis * ADCaxis = (TAxis*) TvAhist->GetYaxis();
  
  int binum= TvAhist->GetXaxis()->FindBin(500);
  int binum2= TvAhist->GetXaxis()->FindBin(750);
  cout <<"first bin#:" <<binum<<"\n";
  cout <<"last bin#:" <<binum2<<"\n";
  //making the graphs (LOOOOOP)
  cout<<"Here\n";
  TGraph *gr1 = new TGraph();
  gr1->SetName("mean_graph of PMT 3");
  
  for (int binn=binum;binn<binum2;binn=binn+1){
    int binn2= binn+1;

    TH1D*g=(TH1D*)TvAhist->ProjectionY("",binn,binn2,"[cutg1],[cutg2]");
    outputFile->cd();

    g->Write();
    g->Draw();

    double d=g->GetMean();
    gr1->SetPoint(gr1->GetN(),TvAhist->GetXaxis()->GetBinCenter(binn),d);
  }
   
  //TF1*myfit=new TF1("fitgraph","([1]/sqrt(x))+[0]",l->GetBinCenter(0),l->GetBinCenter(41));
  TF1 * myfit = new TF1("fit","([1]/sqrt(x))+[0]",500,750);
  
  outputFile->cd();
  //
  
  myfit->SetParameter(0,60);
  myfit->SetParameter(1,30);
  //myfit->Draw();
  
  TFitResultPtr mypoint=gr1->Fit(myfit,"qesrn","",500,750);
  double p0 = mypoint->Parameter(0);
  double p1 = mypoint->Parameter(1);

  cout<<"p0 = :"<<p0<<" p1 = :"<<p1 << "\n";
  
  gr1->Write();

  //TCanvas * c1 = new TCanvas("c1");
  //gr1->Draw();
  //mypoint->Draw("same");
  //c1->Update(); c1->Modified();
  //c1->Write();

    TH2D * TvAhist4 = (TH2D*) inputFile->Get("TvA_4");
  TAxis * ADCaxis4 = (TAxis*) TvAhist->GetYaxis();
  
  int binum3= TvAhist4->GetXaxis()->FindBin(500);
  int binum4= TvAhist4->GetXaxis()->FindBin(750);
  cout <<"first bin#:" <<binum3<<"\n";
  cout <<"last bin#:" <<binum4<<"\n";
  //making the graphs (LOOOOOP)
  cout<<"Here\n";
  TGraph *gr3 = new TGraph();
  gr3->SetName("mean_graph of PMT4");
  
  for (int binn3=binum3;binn3<binum4;binn3=binn3+1){
    int binn4= binn3+1;

    TH1D*g2=(TH1D*)TvAhist4->ProjectionY("",binn3,binn4,"[cutg1],[cutg2]");
    outputFile->cd();

    g2->Write();
    g2->Draw();

    double d2=g2->GetMean();
    gr3->SetPoint(gr3->GetN(),TvAhist4->GetXaxis()->GetBinCenter(binn3),d2);
  }
   
  //TF1*myfit=new TF1("fitgraph","([1]/sqrt(x))+[0]",l->GetBinCenter(0),l->GetBinCenter(41));
  TF1 * myfit2 = new TF1("fit","([1]/sqrt(x))+[0]",500,750);
  
  outputFile->cd();
  //
  
  myfit2->SetParameter(0,60);
  myfit2->SetParameter(1,30);
  //myfit->Draw();
  
  TFitResultPtr mypoint2=gr3->Fit(myfit,"qesrn","",500,750);
  double p02 = mypoint2->Parameter(0);
  double p12 = mypoint2->Parameter(1);

  cout<<"p02 = :"<<p02<<" p12 = :"<<p12 << "\n";
  
  gr3->Write();
  
  return 1;
}


			       

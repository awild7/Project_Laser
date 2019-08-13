#include<iostream>
#include<cmath>
#include<fstream>

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
  if ( argc !=4 ){
    cout <<"Error wrong amount of arguments used\n"
	 <<"timeWalk /path/to/input/hist/file path/to/output/hist/file path/to/output/text/file \n";
    return -1;
  }

  //getting the file
  TFile * inputFile = new TFile(argv[1]);
  TFile * outputFile = new TFile(argv[2], "RECREATE");
  ofstream textFile;
  textFile.open(argv[3]);
  textFile<<"0 0 0\n";
  cout<<"File has been opened from: "<<argv[1]<<"\n";
  
  TH2D * TvAList[4];
  TvAList[0] = (TH2D*) inputFile->Get("TvA_1");
  TvAList[1] = (TH2D*) inputFile->Get("TvA_2");
  TvAList[2] = (TH2D*) inputFile->Get("TvA_3");
  TvAList[3] = (TH2D*) inputFile->Get("TvA_4");

  TGraph * aveGraph[4];
  TGraph * fitGraph[4];
  
  double minA[4];
  double maxA[4];
  double minT[4];
  double maxT[4];
  minA[0]=500;
  minA[1]=550;
  minA[2]=500;
  minA[3]=500;
  maxA[0]=1150;
  maxA[1]=1250;
  maxA[2]=1200;
  maxA[3]=1100;
  minT[0]=30;
  minT[1]=65;
  minT[2]=25;
  minT[3]=50;
  maxT[0]=45;
  maxT[1]=80;
  maxT[2]=40;
  maxT[3]=65;
  //Now Preform the loop of PMTs
  for( int i = 0; i < 4; i++){

    char grName[100];
    sprintf(grName,"Graph_%d",(i+1));
    aveGraph[i] = new TGraph();
    aveGraph[i]->SetName(grName);
  
    int minBin = TvAList[i]->GetXaxis()->FindBin(minA[i]);
    int maxBin = TvAList[i]->GetXaxis()->FindBin(maxA[i]);

    //Now preform loop of ADC bins
    for(int j = minBin; j < maxBin; j++){
      TH1D * proHist = (TH1D*) TvAList[i]->ProjectionY("",j,j+1);

      //Preform the fit to a guassian
      TF1 * gfit = new TF1("myFit","gaus",minT[i],maxT[i]);   //Define Fit
      gfit->SetParameter(0,proHist->GetMaximum());
      gfit->SetParameter(1,proHist->GetMean());
      gfit->SetParameter(2,proHist->GetStdDev());
      TFitResultPtr gpoint = proHist->Fit(gfit,"qeSrn","",minT[i],maxT[i]);
      int fitStatus = gpoint;
      double y = proHist->GetMean();
      if(fitStatus == 0){
	double yt = gpoint->Parameter(1); 
	if((yt>20) && (yt < 80))
	  y =yt;
      }

      double x = TvAList[i]->GetXaxis()->GetBinCenter(j);
      //Get the graph of averages
      aveGraph[i]->SetPoint(aveGraph[i]->GetN(),x,y);
    }

    //Naow make the fit
    TF1 * myfit = new TF1("fit","([1]/sqrt(x))+[0]",minA[i],maxA[i]);
    myfit->SetParameter(0,60);
    myfit->SetParameter(1,30);
    TFitResultPtr mypoint=aveGraph[i]->Fit(myfit,"qesrn","",minA[i],maxA[i]);

    int fitSt = mypoint;
    if(fitSt != 0){
      cout<<"No Pass!! \n\n\n\n\n";
      continue;
    }

    double p0 = mypoint->Parameter(0);
    double p1 = mypoint->Parameter(1);
    textFile<<(i+1)<<" "<<p1<<" "<<p0 << "\n";


    //Now make this fit into a tgraph
    char ftName[100];
    sprintf(ftName,"Fit_%d",(i+1));
    fitGraph[i] = new TGraph();
    fitGraph[i]->SetName(ftName);

    for(int j = 0; j < 1000; j++){

    double x = ((double)j * 3) +1;
    double y = (p1/sqrt(x)) + p0;
    //    cout<<"x: "<<x<<"   y: "<<y<<"\n";
    fitGraph[i]->SetPoint(fitGraph[i]->GetN(),x,y);
    }
    
  }

    textFile<<"5 0 0\n";
    textFile<<"6 0 0\n";
    textFile<<"7 0 0\n";
    textFile<<"8 0 0\n";
    textFile<<"9 0 0\n";
  
  outputFile->cd();
  for(int i = 0; i < 4; i++){
    TvAList[i]->Write();
    aveGraph[i]->Write();
    fitGraph[i]->Write();
  }
  outputFile->Close();
  textFile.close();
  return 1;
}


			       

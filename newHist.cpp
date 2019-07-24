
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h> 
#include <TH1F.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TTree.h>
#include <TFile.h>

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

int stepSizeADC = 1000;
// given by Andrew on Friday April 12th 2019; modified after

int main(int argc, char ** argv){

  if( argc != 4){
    
    cerr<<"Wrong number of arguments. Instead try:\n\t"
	<< "laserHist /path/to/input/Tree/file /path/to/output/Hist/file [run time]\n";

    return -1;
    
  }
  
//Get D and A trees. Open output file.
  TFile * inputFile = new TFile(argv[1]);
  TFile * outputFile = new TFile(argv[2],"RECREATE");
  double timeLength = atoi(argv[3]);
  
  cerr<<"File has been open from: "<<argv[1]<<"\n";
  
//Make trees and histograms for the nuclei
  TTree * TreeH = (TTree*)inputFile->Get("T");

  //Define variables needed for histograms
  double dataA1[99] = {0.};
  double dataA2[99] = {0.};
  double dataA3[99] = {0.};
  double dataA4[99] = {0.};
  double dataA5[99] = {0.};
  double dataA6[99] = {0.};

  double dataT1[99] = {0.};
  double dataT2[99] = {0.};
  double dataT3[99] = {0.};
  double dataT4[99] = {0.};
  double dataT5[99] = {0.};
  double dataT6[99] = {0.};
  double dataT7[99] = {0.};
  
  
  
  //Set addresses for data (D_A00)
  TreeH->SetBranchAddress("D.A01",&dataA1);
  TreeH->SetBranchAddress("D.A02",&dataA2);
  TreeH->SetBranchAddress("D.A03",&dataA3);
  TreeH->SetBranchAddress("D.A04",&dataA4);
  TreeH->SetBranchAddress("D.A05",&dataA5);
  TreeH->SetBranchAddress("D.A06",&dataA6);

  TreeH->SetBranchAddress("D.T001",&dataT1);
  TreeH->SetBranchAddress("D.T002",&dataT2);
  TreeH->SetBranchAddress("D.T003",&dataT3);
  TreeH->SetBranchAddress("D.T004",&dataT4);
  TreeH->SetBranchAddress("D.T005",&dataT5);
  TreeH->SetBranchAddress("D.T006",&dataT6);
  TreeH->SetBranchAddress("D.T007",&dataT7);
			    
  //Now make a vector of graphs for the ADCs and TDCs
         //the TDCs
  vector<TGraphAsymmErrors*> graphTDCList;
  TGraphAsymmErrors * graphT1 = new TGraphAsymmErrors();
  graphT1->SetName("TDC1_time");
  graphTDCList.push_back(graphT1);
  TGraphAsymmErrors * graphT2 = new TGraphAsymmErrors();
  graphT2->SetName("TDC2_time");                      
  graphTDCList.push_back(graphT2);
  TGraphAsymmErrors * graphT3 = new TGraphAsymmErrors();
  graphT3->SetName("TDC3_time");                      
  graphTDCList.push_back(graphT3);
  TGraphAsymmErrors * graphT4 = new TGraphAsymmErrors();
  graphT4->SetName("TDC4_time");
  graphTDCList.push_back(graphT4);
  TGraphAsymmErrors * graphTb1 = new TGraphAsymmErrors();
  graphTb1->SetName("TDCb1_time");                    
  graphTDCList.push_back(graphTb1);
  TGraphAsymmErrors * graphTb2 = new TGraphAsymmErrors();
  graphTb2->SetName("TDCb2_time");                    
  graphTDCList.push_back(graphTb2);
  TGraphAsymmErrors * graphT5 = new TGraphAsymmErrors();
  graphT5->SetName("TDC5_time");                      
  graphTDCList.push_back(graphT5);
  TGraphAsymmErrors * graphT6 = new TGraphAsymmErrors();
  graphT6->SetName("TDC6_time");                                    
  graphTDCList.push_back(graphT6);

           //The ADCs
  vector<TGraphAsymmErrors*> graphADCList;
  TGraphAsymmErrors * graphA1 = new TGraphAsymmErrors();
  graphA1->SetName("ADC1_time");
  graphADCList.push_back(graphA1);
  TGraphAsymmErrors * graphA2 = new TGraphAsymmErrors();
  graphA2->SetName("ADC2_time");                      
  graphADCList.push_back(graphA2);
  TGraphAsymmErrors * graphA3 = new TGraphAsymmErrors();
  graphA3->SetName("ADC3_time");                      
  graphADCList.push_back(graphA3);
  TGraphAsymmErrors * graphA4 = new TGraphAsymmErrors();
  graphA4->SetName("ADC4_time");
  graphADCList.push_back(graphA4);
  TGraphAsymmErrors * graphAb1 = new TGraphAsymmErrors();
  graphAb1->SetName("ADCb1_time");                    
  graphADCList.push_back(graphAb1);
  TGraphAsymmErrors * graphAb2 = new TGraphAsymmErrors();
  graphAb2->SetName("ADCb2_time");                    
  graphADCList.push_back(graphAb2);
  TGraphAsymmErrors * graphA5 = new TGraphAsymmErrors();
  graphA5->SetName("ADC5_time");                      
  graphADCList.push_back(graphA5);
  TGraphAsymmErrors * graphA6 = new TGraphAsymmErrors();
  graphA6->SetName("ADC6_time");                                    
  graphADCList.push_back(graphA6);
 
  
  double eventsADC = (double)(TreeH->GetEntries()/stepSizeADC);

  cerr<<"Histograms and Trees successfully created\n";
  
  //Loop over TTree

  const double A = 0.025;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(int i = 0; i < TreeH->GetEntries(); i += stepSizeADC){
    
     //Display completed
    if((i%10000) == 0){
      cerr << (i*100.)/(TreeH->GetEntries()) <<"% completed events in the first loop \n";
    }
    TreeH->GetEntry(i);

    //Convet TDC to units of picosecons
    double T1 = dataT1[0] * A;
    double T2 = dataT2[0] * A;
    double T3 = dataT3[0] * A;
    double T4 = dataT4[0] * A;
    double T5 = dataT5[0] * A;
    double T6 = dataT6[0] * A;    
    double T7 = dataT7[0] * A;

    double xTDC = (double)i * timeLength / (double)(TreeH->GetEntries());
    graphT1 ->SetPoint(graphT1 ->GetN(),xTDC,dataT1[0]-dataT7[0]);
    graphT2 ->SetPoint(graphT2 ->GetN(),xTDC,dataT2[0]-dataT7[0]);
    graphT3 ->SetPoint(graphT3 ->GetN(),xTDC,dataT3[0]-dataT7[0]);
    graphT4 ->SetPoint(graphT4 ->GetN(),xTDC,dataT4[0]-dataT7[0]);
    graphTb1->SetPoint(graphTb1->GetN(),xTDC,sqrt(dataT1[0]*dataT2[0])-dataT7[0]);
    graphTb2->SetPoint(graphTb2->GetN(),xTDC,sqrt(dataT3[0]*dataT4[0])-dataT7[0]);
    graphT5 ->SetPoint(graphT5 ->GetN(),xTDC,dataT5[0]-dataT7[0]);
    graphT6 ->SetPoint(graphT6 ->GetN(),xTDC,dataT6[0]-dataT7[0]);
  
    double xADC = (double)i * timeLength / (double)(TreeH->GetEntries());
    graphA1 ->SetPoint(graphA1 ->GetN(),xADC,dataA1[0]);
    graphA2 ->SetPoint(graphA2 ->GetN(),xADC,dataA2[0]);
    graphA3 ->SetPoint(graphA3 ->GetN(),xADC,dataA3[0]);
    graphA4 ->SetPoint(graphA4 ->GetN(),xADC,dataA4[0]);
    graphAb1->SetPoint(graphAb1->GetN(),xADC,sqrt(dataA1[0]*dataA2[0]));
    graphAb2->SetPoint(graphAb2->GetN(),xADC,sqrt(dataA3[0]*dataA4[0]));
    graphA5 ->SetPoint(graphA5 ->GetN(),xADC,dataA5[0]);
    graphA6 ->SetPoint(graphA6 ->GetN(),xADC,dataA6[0]);
    //cout << dataA1[0]<<"\n";
    }
  cerr<<"Finished loop over TTree\n";
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   TGraphAsymmErrors * grA1 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grA2 = new TGraphAsymmErrors();
   
  grA1->SetName("ADC_Averages_Graph1");
  grA2->SetName("ADC_Averages_Graph2");
  
 for(int i=0;i<TreeH->GetEntries();i=i+stepSizeADC){
 double xADC = (double)i * timeLength / (double)(TreeH->GetEntries());
 //cout <<i<<"\n";
 double sum = 0;
 
    for(int j=i;j<(i+stepSizeADC);j++){

      TreeH->GetEntry(j);
      sum = sum+dataA1[0];
      
    }
    double mean= sum/stepSizeADC;
    grA1->SetPoint(grA1->GetN(),xADC,mean);
   }
 
 for(int i=0;i<TreeH->GetEntries();i=i+stepSizeADC){
 double xADC = (double)i * timeLength / (double)(TreeH->GetEntries());
 double sum = 0;
 
    for(int j=i;j<(i+stepSizeADC);j++){

      TreeH->GetEntry(j);
      sum = sum+dataA2[0];
      
    }
    double mean= sum/stepSizeADC;
    grA2->SetPoint(grA2->GetN(),xADC,mean);
   }  
  
  
  inputFile->Close();


  //Write out
  outputFile->cd();
    grA1->Write();
    grA2->Write();
  for (int i = 0; i < graphADCList.size(); i++){graphADCList[i]->Write();}
  for (int i = 0; i < graphTDCList.size(); i++){graphTDCList[i]->Write();}
  
  outputFile->Close();
  cerr<< argv[2]<<" has been completed. \n\n\n";

  return 0;
   }

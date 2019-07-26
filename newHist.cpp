
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

int stepSizeADC = 5000;
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
 
  
  double eventsADC = (double)(TreeH->GetEntries()/1000);

  cerr<<"Histograms and Trees successfully created\n";
  
  //Loop over TTree

  const double A = 0.025;
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  for(int i = 0; i < TreeH->GetEntries(); i += 1000){
    
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
    graphT1 ->SetPoint(graphT1 ->GetN(),xTDC,T1-T7);
    graphT2 ->SetPoint(graphT2 ->GetN(),xTDC,T2-T7);
    graphT3 ->SetPoint(graphT3 ->GetN(),xTDC,T3-T7);
    graphT4 ->SetPoint(graphT4 ->GetN(),xTDC,T4-T7);
    graphTb1->SetPoint(graphTb1->GetN(),xTDC,sqrt(T1*T2)-T7);
    graphTb2->SetPoint(graphTb2->GetN(),xTDC,sqrt(T3*T4)-T7);
    graphT5 ->SetPoint(graphT5 ->GetN(),xTDC,T5-T7);
    graphT6 ->SetPoint(graphT6 ->GetN(),xTDC,T6-T7);
  
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
   TGraphAsymmErrors * grA3 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grA4 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grA5 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grA6 = new TGraphAsymmErrors();
   
  grA1->SetName("ADC_Averages_Graph1");
  grA2->SetName("ADC_Averages_Graph2");
  grA3->SetName("ADC_Averages_Graph3");
  grA4->SetName("ADC_Averages_Graph4");
  grA5->SetName("ADC_Averages_Graph5");
  grA6->SetName("ADC_Averages_Graph6");

   TGraphAsymmErrors * grT1 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grT2 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grT3 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grT4 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grT5 = new TGraphAsymmErrors();
   TGraphAsymmErrors * grT6 = new TGraphAsymmErrors();
   
  grT1->SetName("TDC_Averages_Graph1");
  grT2->SetName("TDC_Averages_Graph2");
  grT3->SetName("TDC_Averages_Graph3");
  grT4->SetName("TDC_Averages_Graph4");
  grT5->SetName("TDC_Averages_Graph5");
  grT6->SetName("TDC_Averages_Graph6");


  
 for(int i=0;i<TreeH->GetEntries();i=i+stepSizeADC){
 double sumA = 0;
 double sumT = 0;

 double sumA2 = 0;
 double sumT2 = 0;

 double sumA3 = 0;
 double sumT3 = 0;

 double sumA4 = 0;
 double sumT4 = 0;

 double sumA5 = 0;
 double sumT5 = 0;

 double sumA6 = 0;
 double sumT6 = 0;

 double xADC = (double)i * timeLength / (double)(TreeH->GetEntries());
 double xTDC = (double)i * timeLength / (double)(TreeH->GetEntries());

 

 for(int j=i;j<(i+stepSizeADC);j++){

      TreeH->GetEntry(j);


    double T1 = dataT1[0] * A;
    double T2 = dataT2[0] * A;
    double T3 = dataT3[0] * A;
    double T4 = dataT4[0] * A;
    double T5 = dataT5[0] * A;
    double T6 = dataT6[0] * A;    
    double T7 = dataT7[0] * A;
 
      sumA = sumA+dataA1[0];
      sumT = sumT+(T1-T7);

      sumA2 = sumA2+dataA2[0];
      sumT2 = sumT2+(T2-T7);

      sumA3 = sumA3+dataA3[0];
      sumT3 = sumT3+(T3-T7);

      sumA4 = sumA4+dataA4[0];
      sumT4 = sumT4+(T4-T7);

      sumA5 = sumA5+dataA5[0];
      sumT5 = sumT5+(T5-T7);

      sumA6 = sumA6+dataA6[0];
      sumT6 = sumT6+(T6-T7);
    }
    
    double meanA= sumA/stepSizeADC;
    grA1->SetPoint(grA1->GetN(),xADC,meanA);

    double meanT= sumT/stepSizeADC;
    grT1->SetPoint(grT1->GetN(),xTDC,meanT); 

    double meanA2= sumA2/stepSizeADC;
    grA2->SetPoint(grA2->GetN(),xADC,meanA2);

    double meanT2= sumT2/stepSizeADC;
    grT2->SetPoint(grT2->GetN(),xTDC,meanT2);

     double meanA3= sumA3/stepSizeADC;
    grA3->SetPoint(grA3->GetN(),xADC,meanA3);

    double meanT3= sumT3/stepSizeADC;
    grT3->SetPoint(grT3->GetN(),xTDC,meanT3);

    double meanA4= sumA4/stepSizeADC;
    grA4->SetPoint(grA4->GetN(),xADC,meanA4);

    double meanT4= sumT4/stepSizeADC;
    grT4->SetPoint(grT4->GetN(),xTDC,meanT4);

    double meanA5= sumA5/stepSizeADC;
    grA5->SetPoint(grA5->GetN(),xADC,meanA5);

    double meanT5= sumT5/stepSizeADC;
    grT5->SetPoint(grT5->GetN(),xTDC,meanT5);

    double meanA6= sumA6/stepSizeADC;
    grA6->SetPoint(grA6->GetN(),xADC,meanA6);

    double meanT6= sumT6/stepSizeADC;
    grT6->SetPoint(grT6->GetN(),xTDC,meanT6);
    
 }
 

  
  
  inputFile->Close();


  //Write out
  outputFile->cd();
    grA1->Write();
    grA2->Write();
    grA3->Write();
    grA4->Write();
    grA5->Write();
    grA6->Write();

    grT1->Write();
    grT2->Write();
    grT3->Write();
    grT4->Write();
    grT5->Write();
    grT6->Write();
    
  for (int i = 0; i < graphADCList.size(); i++){graphADCList[i]->Write();}
  for (int i = 0; i < graphTDCList.size(); i++){graphTDCList[i]->Write();}
  
  outputFile->Close();
  cerr<< argv[2]<<" has been completed. \n\n\n";

  return 0;
   }

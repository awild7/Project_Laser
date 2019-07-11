
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h> 

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

double minRes = -100;
double maxRes = 100;

int stepSizeADC = 1;
int stepSizeSlice = 1000;
double timeLength = 13.88;


// given by Andrew on Friday April 12th 2019; modified after

int main(int argc, char ** argv){

  if( argc != 5){
    
    cerr<<"Wrong number of arguments. Instead try:\n\t"
	<< "laserHist /path/to/input/Tree/file /path/to/output/Hist/file [c1] [c0]\n";

    return -1;
    
  }
  
//Get D and A trees. Open output file.
  TFile * inputFile = new TFile(argv[1]);
  TFile * outputFile = new TFile(argv[2],"RECREATE");
  double c1 = atoi(argv[3]);
  double c0 = atoi(argv[4]);
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




  
  
  //Histograms we want to output
  vector<TH1*> totalHistList1D;
  vector<TH2*> totalHistList2D;
  TH1D * ToF_res_bar1 = new TH1D("ToFTop","Time of Flight on Top Bar",4000,0,100);
  totalHistList1D.push_back(ToF_res_bar1);
  TH1D * ToF_res_bar2 = new TH1D("ToFBot","Time of Flight on Bottom Bar",4000,0,100);
  totalHistList1D.push_back(ToF_res_bar2);
  TH1D * Pos_res_bar1 = new TH1D("PosTop","Time of Flight on Top Bar",4000,-100,100);
  totalHistList1D.push_back(Pos_res_bar1);
  TH1D * Pos_res_bar2 = new TH1D("PosBot","Time of Flight on Bottom Bar",4000,-100,100);  
  totalHistList1D.push_back(Pos_res_bar2);
  TH1D * ToF_res_box = new TH1D("ToFbox","Time of Flight on PMT Box",4000,0,100);
  totalHistList1D.push_back(ToF_res_box);
  TH1D * ToF_res_PD = new TH1D("ToFPD","Time of Flight on Photodiode",40,27,28);
  totalHistList1D.push_back(ToF_res_PD);
  TH2D * ToF_v_ADC_bar1 = new TH2D("TvA_1","Time of Flight vs ADC Top Bar",90,550,3000,8000,0,100);
  totalHistList2D.push_back(ToF_v_ADC_bar1);
  TH2D * ToF_v_ADC_bar2 = new TH2D("TvA_2","Time of Flight vs ADC Bottom Bar",90,550,3000,8000,0,100);
  totalHistList2D.push_back(ToF_v_ADC_bar2);
  TH2D * ToF_v_ADC_3 = new TH2D("TvA_3","Time of Flight vs ADC of PMT 3",150,0,3000,8000,-60,60);
  totalHistList2D.push_back(ToF_v_ADC_3);
  TH2D * ToF_v_ADC_4 = new TH2D("TvA_4","Time of Flight vs ADC of PMT 4",150,0,3000,8000,-60,60);
  totalHistList2D.push_back(ToF_v_ADC_4);

  //Now make a vector of graphs for the ADCs and time of flights
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
  //Time of Flights
  TGraphAsymmErrors * graphToF1 = new TGraphAsymmErrors();
  graphToF1->SetName("ToFb1_time");                     
  graphADCList.push_back(graphToF1);
  TGraphAsymmErrors * graphToF2 = new TGraphAsymmErrors();
  graphToF2->SetName("ToFb2_time");                                    
  graphADCList.push_back(graphToF2);
  
  
  double eventsADC = (double)(TreeH->GetEntries()/stepSizeADC);

  cerr<<"Histograms and Trees successfully created\n";
  
//Loop over TTree

  const double A = 0.025;

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

    /* if(sqrt(dataA1[0]*dataA2[0])<550){
      continue;
    }

    if(sqrt(dataA3[0]*dataA4[0])<550){
       continue;
    }
    */
    
    double xADC = (double)i * timeLength / (double)(TreeH->GetEntries());
    graphA1 ->SetPoint(graphA1 ->GetN(),xADC,dataA1[0]);
    graphA2 ->SetPoint(graphA2 ->GetN(),xADC,dataA2[0]);
    graphA3 ->SetPoint(graphA3 ->GetN(),xADC,dataA3[0]);
    graphA4 ->SetPoint(graphA4 ->GetN(),xADC,dataA4[0]);
    graphAb1->SetPoint(graphAb1->GetN(),xADC,sqrt(dataA1[0]*dataA2[0]));
    graphAb2->SetPoint(graphAb2->GetN(),xADC,sqrt(dataA3[0]*dataA4[0]));
    graphA5 ->SetPoint(graphA5 ->GetN(),xADC,dataA5[0]);
    graphA6 ->SetPoint(graphA6 ->GetN(),xADC,dataA6[0]);
    
    graphToF1 ->SetPoint(graphA6 ->GetN(),xADC,((T1+T2)/2)-T7);
    graphToF2 ->SetPoint(graphA6 ->GetN(),xADC,((T3+T4)/2)-T7);
    
    //Fill in total histograms for only the correct step sizes
    ToF_res_bar1->Fill(((T1+T2)/2)-T7);
    ToF_res_bar2->Fill(((T3+T4)/2)-T7);
    Pos_res_bar1->Fill(T1-T2);
    Pos_res_bar2->Fill(T3-T4);
    ToF_res_box->Fill(T5-T7);
    ToF_res_PD->Fill(T6-T7);
    
    ToF_v_ADC_bar1->Fill(((dataA1[0]+dataA2[0])/2),((T1+T2)/2)-T7);
    ToF_v_ADC_bar2->Fill(((dataA3[0]+dataA4[0])/2),((T3+T4)/2)-T7);
    ToF_v_ADC_3->Fill(dataA3[0],(T3-T7)-(c1/sqrt(dataA3[0]))-c0);
    ToF_v_ADC_4->Fill(dataA4[0],(T4-T7)-(c1/sqrt(dataA4[0]))-c0);
    
    }
     
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  //Now make a vector of graphs for the resolution graphs
  vector<TGraphAsymmErrors*> graphResList;
  TGraphAsymmErrors * graphresT1 = new TGraphAsymmErrors();
  graphresT1->SetName("ToF_res_1_time");
  graphResList.push_back(graphresT1);
  TGraphAsymmErrors * graphresT2 = new TGraphAsymmErrors();
  graphresT2->SetName("ToF_res_2_time");
  graphResList.push_back(graphresT2);
  TGraphAsymmErrors * graphresT3 = new TGraphAsymmErrors();
  graphresT3->SetName("ToF_res_3_time");
  graphResList.push_back(graphresT3);
  TGraphAsymmErrors * graphresT4 = new TGraphAsymmErrors();
  graphresT4->SetName("ToF_res_4_time");
  graphResList.push_back(graphresT4);
  TGraphAsymmErrors * graphresb1 = new TGraphAsymmErrors();
  graphresb1->SetName("Pos_res_b1_time");
  graphResList.push_back(graphresb1);
  TGraphAsymmErrors * graphresb2 = new TGraphAsymmErrors();
  graphresb2->SetName("Pos_res_b2_time");
  graphResList.push_back(graphresb2);
  TGraphAsymmErrors * graphresT5 = new TGraphAsymmErrors();
  graphresT5->SetName("ToF_res_5_time");
  graphResList.push_back(graphresT5);
  TGraphAsymmErrors * graphresT6 = new TGraphAsymmErrors();
  graphresT6->SetName("ToF_res_6_time");
  graphResList.push_back(graphresT6);

  //Start by defining histograms to get the standard deviations
  vector<TH1*> sliceHistList;
  TH1D * slice_T1 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_T1);
  TH1D * slice_T2 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_T2);
  TH1D * slice_T3 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_T3);
  TH1D * slice_T4 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_T4);
  TH1D * slice_b1 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_b1);
  TH1D * slice_b2 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_b2);
  TH1D * slice_T5 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_T5);
  TH1D * slice_T6 = new TH1D("sliceT1","sliceT1",4000,minRes,maxRes);
  sliceHistList.push_back(slice_T6);

  for(int i = 0; i < TreeH->GetEntries(); i += stepSizeSlice){

    //Display completed
    if((i%10000) == 0){
      cerr << (i*100.)/(TreeH->GetEntries()) <<"% completed events in the second loop \n";
    }  
    for(int j = i; j < (i + stepSizeSlice); j++){         
      if(j >= TreeH->GetEntries()){
	continue;
      }
      //Get the entries at j
      TreeH->GetEntry(j);
      //Convet TDC to units of picosecons
      double T1 = dataT1[0] * A;
      double T2 = dataT2[0] * A;
      double T3 = dataT3[0] * A;
      double T4 = dataT4[0] * A;
      double T5 = dataT5[0] * A;
      double T6 = dataT6[0] * A;    
      double T7 = dataT7[0] * A;	 	 
      //now calculate the resolution of time with the slice histograms and then fill the vectors    
      //Now fill histograms
      slice_T1->Fill(T1-T7);
      slice_T2->Fill(T2-T7);
      slice_T3->Fill(T3-T7);
      slice_T4->Fill(T4-T7);
      slice_b1->Fill(T1-T2);
      slice_b2->Fill(T3-T4);
      slice_T5->Fill(T5-T7);
      slice_T6->Fill(T6-T7);
    }

    double xSlice = (double)i * timeLength / ((double)(TreeH->GetEntries()));
    /* 
    //Now I can fit my slices to my gausians
    for (int k = 0; k < sliceHistList.size(); k++){
      //Fit to gaus
      sliceHistList[k]->Fit("gaus","qer","",((sliceHistList[k]->GetMean())-1.5),((sliceHistList[k]->GetMean())+1.5));
      //Fill vector
      graphResList[k]->SetPoint(graphResList[k]->GetN(),xSlice,sliceHistList[k]->GetFunction("gaus")->GetParameter(2));
      //Reset histogram
      sliceHistList[k]->Reset();
    }
    */
  }
    
  cerr<<"Finished loop over TTree\n";
 
  inputFile->Close();


  //Write out
  outputFile->cd();  
  for (int i = 0; i < totalHistList1D.size(); i++){totalHistList1D[i]->Write();}
  for (int i = 0; i < totalHistList2D.size(); i++){totalHistList2D[i]->Write();}
  for (int i = 0; i < graphADCList.size(); i++){graphADCList[i]->Write();}
  for (int i = 0; i < graphResList.size(); i++){graphResList[i]->Write();}
  outputFile->Close();
  cerr<< argv[2]<<" has been completed. \n\n\n";
  
  return 0;
}


       /*
       // If I need a more acurate fit
       cout<<"THis Far";
    //Code to set parameters beforehand
    TF1 * T6fit = new TF1("T6fit","gaus",minRes,maxRes);
    
    T6fit->SetParameter(0,slice_T6->GetMaximum());
    T6fit->SetParameter(1,slice_T6->GetMean());
    T6fit->SetParameter(2,0.2);
    TFitResultPtr point = slice_T6->Fit(T6fit,"qesr","",minRes,maxRes);
    resT6.push_back(point->Parameter(2));
       */


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
int stepSizeADC = 10;

void failReadIn(){
  cerr<<"the input text file should be constructed\n"
      <<"with three rows for the PMT number, the "
      <<"first parameter p1, and the second parameter"
      <<"p0. It should start at PMT 0 and go to 9\n\n"
      <<"0 [p1 of PMT 0] [p0 of PMT 0]\n"
      <<"1 [p1 of PMT 1] [p0 of PMT 1]\n"
      <<".\n"
      <<".\n"
      <<".\n"
      <<"9 [p1 of PMT 9] [p0 of PMT 9]\n";
  exit(-1);
}
// given by Andrew on Friday April 12th 2019; modified after

int main(int argc, char ** argv){

  if( argc != 5){
    
    cerr<<"Wrong number of arguments. Instead try:\n\t"
	<< "laserHist /path/to/input/Tree/file /path/to/input/parameter/text/file /path/to/output/Hist/file [time length] \n";

    return -1;
    
  }
  
//Get D and A trees. Open output file.
  TFile * inputFile = new TFile(argv[1]);
  ifstream inParamFile;
  inParamFile.open(string(argv[2]));
  TFile * outputFile = new TFile(argv[3],"RECREATE");
  double timeLength = atoi(argv[4]);
  int counter=0;
  int PMTnumber;
  double c1[10];
  double c0[10];
  while(inParamFile >> PMTnumber){
    if(counter != PMTnumber){
      failReadIn();
    }
    inParamFile >> c1[PMTnumber] >> c0[PMTnumber];
    counter++;
  } 
   
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

  vector<TH2*> totalHistList2D;
  TH2D * ToF_v_ADC_1 = new TH2D("TvA_1","Time of Flight vs ADC of PMT 1",150,0,3000,8000,0-100,100);
  totalHistList2D.push_back(ToF_v_ADC_1);
  TH2D * ToF_v_ADC_2 = new TH2D("TvA_2","Time of Flight vs ADC of PMT 2",150,0,3000,8000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_2);
  TH2D * ToF_v_ADC_3 = new TH2D("TvA_3","Time of Flight vs ADC of PMT 3",150,0,3000,8000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_3);
  TH2D * ToF_v_ADC_4 = new TH2D("TvA_4","Time of Flight vs ADC of PMT 4",150,0,3000,8000,-100,100);
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
  TGraphAsymmErrors * graphA5 = new TGraphAsymmErrors();
  graphA5->SetName("ADC5_time");                      
  graphADCList.push_back(graphA5);
  TGraphAsymmErrors * graphA6 = new TGraphAsymmErrors();
  graphA6->SetName("ADC6_time");                                    
  graphADCList.push_back(graphA6);
  TGraphAsymmErrors * graphAbar1 = new TGraphAsymmErrors();
  graphAbar1->SetName("ADC_bar1_time");                    
  graphADCList.push_back(graphAbar1);
  TGraphAsymmErrors * graphAbar2 = new TGraphAsymmErrors();
  graphAbar2->SetName("ADC_bar2_time");                    
  graphADCList.push_back(graphAbar2);

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

    //Make Time of Flight corrections for PMT 3 and 4 alone for now
    T3 -= (c1[3]/sqrt(dataA3[0])) + c0[3];
    T4 -= (c1[4]/sqrt(dataA4[0])) + c0[4];
    
    double x = (double)i * timeLength / (double)(TreeH->GetEntries());
    graphA1 ->SetPoint(graphA1 ->GetN(),x,dataA1[0]);
    graphA2 ->SetPoint(graphA2 ->GetN(),x,dataA2[0]);
    graphA3 ->SetPoint(graphA3 ->GetN(),x,dataA3[0]);
    graphA4 ->SetPoint(graphA4 ->GetN(),x,dataA4[0]);
    graphA5 ->SetPoint(graphA5 ->GetN(),x,dataA5[0]);
    graphA6 ->SetPoint(graphA6 ->GetN(),x,dataA6[0]);
    graphAbar1->SetPoint(graphAbar1->GetN(),x,sqrt(dataA1[0]*dataA2[0]));
    graphAbar2->SetPoint(graphAbar2->GetN(),x,sqrt(dataA3[0]*dataA4[0]));
    
    graphToF1 ->SetPoint(graphA6 ->GetN(),x,T3-T7);
    graphToF2 ->SetPoint(graphA6 ->GetN(),x,T4-T7);
    
    //Fill in total histograms for only the correct step sizes
    ToF_res_bar1->Fill(((T1+T2)/2)-T7);
    ToF_res_bar2->Fill(((T3+T4)/2)-T7);
    Pos_res_bar1->Fill(T1-T2);
    Pos_res_bar2->Fill(T3-T4);
    ToF_res_box->Fill(T5-T7);
    ToF_res_PD->Fill(T6-T7);
    
    ToF_v_ADC_1->Fill(dataA1[0],T1-T7);
    ToF_v_ADC_2->Fill(dataA2[0],T2-T7);
    ToF_v_ADC_3->Fill(dataA3[0],T3-T7);
    ToF_v_ADC_4->Fill(dataA4[0],T4-T7);
    
    }
     
  cerr<<"Finished loop over TTree\n";
 
  inputFile->Close();


  //Write out
  outputFile->cd();  
  for (int i = 0; i < totalHistList1D.size(); i++){totalHistList1D[i]->Write();}
  for (int i = 0; i < totalHistList2D.size(); i++){totalHistList2D[i]->Write();}
  for (int i = 0; i < graphADCList.size(); i++){graphADCList[i]->Write();}
  outputFile->Close();
  cerr<< argv[2]<<" has been completed. \n\n\n";
  
  return 0;
}


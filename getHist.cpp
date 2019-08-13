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
int stepSize = 1;

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
	<< "getHist /path/to/input/Tree/file /path/to/input/parameter/text/file /path/to/output/Hist/file [time length] \n";

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

  if( !inParamFile.is_open() ){
    cerr<<"The text file could not be opened.\n Aborting...\n\n";
    exit(-1);
  }
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
  TH1D * ToF_res_bar1 = new TH1D("ToFTop","Time of Flight on Top Bar",4000,-100,100);
  totalHistList1D.push_back(ToF_res_bar1);
  TH1D * ToF_res_bar2 = new TH1D("ToFBot","Time of Flight on Bottom Bar",4000,-100,100);
  totalHistList1D.push_back(ToF_res_bar2);
  TH1D * Pos_res_bar1 = new TH1D("PosTop","Time of Flight on Top Bar",4000,-100,100);
  totalHistList1D.push_back(Pos_res_bar1);
  TH1D * Pos_res_bar2 = new TH1D("PosBot","Time of Flight on Bottom Bar",4000,-100,100);  
  totalHistList1D.push_back(Pos_res_bar2);
  TH1D * ToF_res_1 = new TH1D("ToF_PMT1","Time of Flight on PMT 1",4000,-100,100);
  totalHistList1D.push_back(ToF_res_1);
  TH1D * ToF_res_2 = new TH1D("ToF_PMT2","Time of Flight on PMT 2",4000,-100,100);
  totalHistList1D.push_back(ToF_res_2);
  TH1D * ToF_res_3 = new TH1D("ToF_PMT3","Time of Flight on PMT 3",4000,-100,100);
  totalHistList1D.push_back(ToF_res_3);
  TH1D * ToF_res_4 = new TH1D("ToF_PMT4","Time of Flight on PMT 4",4000,-100,100);
  totalHistList1D.push_back(ToF_res_4);
  TH1D * ToF_res_box = new TH1D("ToFbox","Time of Flight on PMT Box",4000,-100,100);
  totalHistList1D.push_back(ToF_res_box);
  TH1D * ToF_res_PD = new TH1D("ToFPD","Time of Flight on Photodiode",4000,-100,100);
  totalHistList1D.push_back(ToF_res_PD);

  vector<TH2*> totalHistList2D;
  TH2D * ToF_v_ADC_1 = new TH2D("TvA_1","Time of Flight vs ADC of PMT 1",450,0,3000,2000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_1);
  TH2D * ToF_v_ADC_2 = new TH2D("TvA_2","Time of Flight vs ADC of PMT 2",450,0,3000,2000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_2);
  TH2D * ToF_v_ADC_3 = new TH2D("TvA_3","Time of Flight vs ADC of PMT 3",450,0,3000,2000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_3);
  TH2D * ToF_v_ADC_4 = new TH2D("TvA_4","Time of Flight vs ADC of PMT 4",450,0,3000,2000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_4);
  TH2D * ToF_v_ADC_5 = new TH2D("TvA_5","Time of Flight vs ADC of PMT 5",450,0,3000,2000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_5);
  TH2D * ToF_v_ADC_6 = new TH2D("TvA_6","Time of Flight vs ADC of PD",450,0,3000,2000,-100,100);
  totalHistList2D.push_back(ToF_v_ADC_6);
  
  cerr<<"Histograms and Trees successfully created\n";
  
//Loop over TTree

  const double A = 0.025;

  for(int i = 0; i < (TreeH->GetEntries()); i += stepSize){
    
    //Display completed
    if((i%10000) == 0){
      cerr << (i*100.)/(TreeH->GetEntries()) <<"% complete in the Histogram Loop \n";
    }

    
    TreeH->GetEntry(i);
    double T1 = ( dataT1[0] * A ) - ( (c1[1]/sqrt(dataA1[0])) + c0[1] );
    double T2 = ( dataT2[0] * A ) - ( (c1[2]/sqrt(dataA2[0])) + c0[2] );
    double T3 = ( dataT3[0] * A ) - ( (c1[3]/sqrt(dataA3[0])) + c0[3] );
    double T4 = ( dataT4[0] * A ) - ( (c1[4]/sqrt(dataA4[0])) + c0[4] );
    double T5 = ( dataT5[0] * A ) - ( (c1[5]/sqrt(dataA5[0])) + c0[5] );
    double T6 = ( dataT6[0] * A ) - ( (c1[6]/sqrt(dataA6[0])) + c0[6] );    
    double T7 = ( dataT7[0] * A );
    
    ToF_res_bar1->Fill(((T1+T2)/2)-T7);
    ToF_res_bar2->Fill(((T3+T4)/2)-T7);
    Pos_res_bar1->Fill(T1-T2);
    Pos_res_bar2->Fill(T3-T4);
    ToF_res_1->Fill(T1-T7);
    ToF_res_2->Fill(T2-T7);
    ToF_res_3->Fill(T3-T7);
    ToF_res_4->Fill(T4-T7);
    ToF_res_box->Fill(T5-T7);
    ToF_res_PD->Fill(T6-T7);
    //    cout<<dataA1[0]<<"   "<<dataT1[0]<<"\n";
    ToF_v_ADC_1->Fill(dataA1[0],T1-T7);
    ToF_v_ADC_2->Fill(dataA2[0],T2-T7);
    ToF_v_ADC_3->Fill(dataA3[0],T3-T7);
    ToF_v_ADC_4->Fill(dataA4[0],T4-T7);
    ToF_v_ADC_5->Fill(dataA5[0],T5-T7);
    ToF_v_ADC_6->Fill(dataA6[0],T6-T7);
    
    }
     
  cerr<<"Finished loop over TTree\n";
 
  inputFile->Close();


  //Write out
  outputFile->cd();  
  for (int i = 0; i < totalHistList1D.size(); i++){totalHistList1D[i]->Write();}
  for (int i = 0; i < totalHistList2D.size(); i++){totalHistList2D[i]->Write();}
  outputFile->Close();
  cerr<< argv[2]<<" has been completed. \n\n\n";
  
  return 0;
}


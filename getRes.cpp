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

double minRes = -1;
double maxRes = 1;
int stepSize = 300000;

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
	<< "getRes  /path/to/input/Tree/file /path/to/input/parameter/text/file /path/to/output/Hist/file [time length] \n";

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
  TGraphAsymmErrors * graphresT5 = new TGraphAsymmErrors();
  graphresT5->SetName("ToF_res_5_time");
  graphResList.push_back(graphresT5);
  TGraphAsymmErrors * graphresT6 = new TGraphAsymmErrors();
  graphresT6->SetName("ToF_res_6_time");
  graphResList.push_back(graphresT6);

  //Start by defining histograms to get the standard deviations
  vector<TH1*> sliceHistList;
  TH1D * slice_T1 = new TH1D("sliceT1","sliceT1",40,minRes,maxRes);
  sliceHistList.push_back(slice_T1);
  TH1D * slice_T2 = new TH1D("sliceT1","sliceT1",40,minRes,maxRes);
  sliceHistList.push_back(slice_T2);
  TH1D * slice_T3 = new TH1D("sliceT1","sliceT1",40,minRes,maxRes);
  sliceHistList.push_back(slice_T3);
  TH1D * slice_T4 = new TH1D("sliceT1","sliceT1",40,minRes,maxRes);
  sliceHistList.push_back(slice_T4);
  TH1D * slice_T5 = new TH1D("sliceT1","sliceT1",40,minRes,maxRes);
  sliceHistList.push_back(slice_T5);
  TH1D * slice_T6 = new TH1D("sliceT1","sliceT1",40,minRes,maxRes);
  sliceHistList.push_back(slice_T6);

  const double A = 0.025;
  
  for(int i = 0; i < (TreeH->GetEntries()); i += stepSize){

    //Display completed
    if((i%(stepSize*10)) == 0){
      cerr << (i*100.)/(TreeH->GetEntries()) <<"% complete\n";
    }  
    for(int j = i; j < (i + stepSize); j++){         
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

      //Make Time of Flight corrections
      T1 -= (c1[1]/sqrt(dataA1[0])) + c0[1];
      T2 -= (c1[2]/sqrt(dataA2[0])) + c0[2];
      T3 -= (c1[3]/sqrt(dataA3[0])) + c0[3];
      T4 -= (c1[4]/sqrt(dataA4[0])) + c0[4];
      T5 -= (c1[5]/sqrt(dataA5[0])) + c0[5];
      T6 -= (c1[6]/sqrt(dataA6[0])) + c0[6];
      
      slice_T3->Fill(T3-T7);
      slice_T4->Fill(T4-T7);
      slice_T1->Fill(T1-T7);
      slice_T2->Fill(T2-T7);
      slice_T5->Fill(T5-T7);
      slice_T6->Fill(T6-T7);
      }

    double xSlice = (double)i * timeLength / ((double)(TreeH->GetEntries()));

    
    //Now I can fit my slices to my gausians
    for (int k = 0; k < sliceHistList.size(); k++){
      TF1 * myfit = new TF1("myFit","gaus",minRes,maxRes);
      myfit->SetParameter(0,sliceHistList[k]->GetMaximum());
      myfit->SetParameter(1,sliceHistList[k]->GetMean());
      myfit->SetParameter(2,sliceHistList[k]->GetStdDev());

      if(k=0){
	sliceHistList[k]->Write();
      }
      
      if( (sliceHistList[k]->GetEntries()) > 1000 ){
	TFitResultPtr point = sliceHistList[k]->Fit(myfit,"qeSrn","",minRes,maxRes);
	int fitStatus = point;
	double res;
	if(fitStatus == 0){
	  res = point->Parameter(2);
	}
	else{
	  res = sliceHistList[k]->GetMean();	
	}
	graphResList[k]->SetPoint(graphResList[k]->GetN(),xSlice,res);
      }

      /*
      int fitStatus = point;
      double res = -1000;      	
      if(fitStatus == 0){
	cout<<"lsakjfsalkfj\n";

	}*/
     
      sliceHistList[k]->Reset();
    }
  }
    
  cerr<<"Finished loop over TTree\n";
 
  inputFile->Close();


  //Write out
  outputFile->cd();  
  for (int i = 0; i < graphResList.size(); i++){graphResList[i]->Write();}
  outputFile->Close();
  cerr<< argv[2]<<" has been completed. \n\n\n";
  
  return 0;
}

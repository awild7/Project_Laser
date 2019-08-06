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

int stepSize = 10000;
int np = 7;
int ng = 7;
const double A = 0.025;

void failReadIn(){
  cerr<<"the input text file should be constructed\n"
      <<"with three rows for the PMT number, the "
      <<"first parameter p1, and the second parameter"
      <<"p0. It should start at PMT 0 and go to 9\n\n"
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
  double c1[9];
  double c0[9];

  if( !inParamFile.is_open() ){
    cerr<<"The text file could not be opened.\n Aborting...\n\n";
    exit(-1);
  }
  while(inParamFile >> PMTnumber){
    if(counter != PMTnumber){
      failReadIn();
    }
    inParamFile >> c1[counter] >> c0[counter];
    counter++;
  } 
   
  cerr<<"File has been open from: "<<argv[1]<<"\n";
  
//Make trees and histograms for the nuclei
  TTree * TreeH = (TTree*)inputFile->Get("T");

  
  //Define variables needed for histograms
  double dA[np];
  double dataA0[99] = {0.};
  double dataA1[99] = {0.};
  double dataA2[99] = {0.};
  double dataA3[99] = {0.};
  double dataA4[99] = {0.};
  double dataA5[99] = {0.};
  double dataA6[99] = {0.};

  double dT[np];
  double dataT0[99] = {0.};
  double dataT1[99] = {0.};
  double dataT2[99] = {0.};
  double dataT3[99] = {0.};
  double dataT4[99] = {0.};
  double dataT5[99] = {0.};
  double dataT6[99] = {0.};
  double dataT7[99] = {0.};
  
  
  
  //Set addresses for data (D_A00)
  TreeH->SetBranchAddress("D.A00",&dataA0);
  TreeH->SetBranchAddress("D.A01",&dataA1);
  TreeH->SetBranchAddress("D.A02",&dataA2);
  TreeH->SetBranchAddress("D.A03",&dataA3);
  TreeH->SetBranchAddress("D.A04",&dataA4);
  TreeH->SetBranchAddress("D.A05",&dataA5);
  TreeH->SetBranchAddress("D.A06",&dataA6);

  TreeH->SetBranchAddress("D.T000",&dataT0);
  TreeH->SetBranchAddress("D.T001",&dataT1);
  TreeH->SetBranchAddress("D.T002",&dataT2);
  TreeH->SetBranchAddress("D.T003",&dataT3);
  TreeH->SetBranchAddress("D.T004",&dataT4);
  TreeH->SetBranchAddress("D.T005",&dataT5);
  TreeH->SetBranchAddress("D.T006",&dataT6);
  TreeH->SetBranchAddress("D.T007",&dataT7);

  //Now make a vector of graphs for the resolution graphs
  TGraphAsymmErrors * graphTimeList[ng];
  TGraphAsymmErrors * graphResList[ng];
  TH1D * coarseHistList[ng];
  
  for(int k = 0; k < ng; k++){

    char tempGT[100];
    sprintf(tempGT,"ToF_%d_time",k);
    graphTimeList[k] = new TGraphAsymmErrors();
    graphTimeList[k]->SetName(tempGT);

    char tempGR[100];
    sprintf(tempGR,"ToF_Res_%d_time",k);
    graphResList[k] = new TGraphAsymmErrors();
    graphResList[k]->SetName(tempGR);

    char tempC[100];
    sprintf(tempC,"coarse_hist_%d",k);
    coarseHistList[k] = new TH1D(tempC,tempC,400,-100,100);
  }
  

  //Loop over every stepSize points
  for(int i = 0; i < (TreeH->GetEntries()); i += stepSize){

    //Display completed
    cerr << (i*100.)/(TreeH->GetEntries()) <<"% complete of averaged loops\n";

        //First find the mean and average coarsly and then look closely
    for(int j = i; j < (i + stepSize); j++){         
      if(j >= TreeH->GetEntries()){
	continue;
      }      

      TreeH->GetEntry(j); //Get the entries at j
      //Define the variables in the arrays
      dT[0] = dataT0[0];
      dT[1] = dataT1[0];
      dT[2] = dataT2[0];
      dT[3] = dataT3[0];
      dT[4] = dataT4[0];
      dT[5] = dataT5[0];
      dT[6] = dataT6[0];

      dA[0] = dataA0[0];
      dA[1] = dataA1[0];
      dA[2] = dataA2[0];
      dA[3] = dataA3[0];
      dA[4] = dataA4[0];
      dA[5] = dataA5[0];
      dA[6] = dataA6[0];
      
      double Tref = dataT7[0] * A;	 	 
      for(int k = 0; k < np; k++){
	double time = dT[k] * A;              //Convert to picoseconds
	time -= (c1[k]/sqrt(dA[k])) + c0[k];  //Time walk correction
	time -= Tref;                           //Reference time
	coarseHistList[k]->Fill(time);         //Fill histogram
      }
    }

    //Define narrow histograms
    TH1D * fineHistList[ng];
    double minFine[ng];
    double maxFine[ng];
    for(int k = 0; k < ng; k++){
      char tempF[100];
      sprintf(tempF,"fine_hist_%d",k);
      minFine[k] = (coarseHistList[k]->GetMean()) - (4 * (coarseHistList[k]->GetStdDev()));
      maxFine[k] = (coarseHistList[k]->GetMean()) + (4 * (coarseHistList[k]->GetStdDev()));
      fineHistList[k] = new TH1D(tempF,tempF,400,minFine[k],maxFine[k]);
    }
    
    //Now run the loop again while looking more narrowly
    for(int j = i; j < (i + stepSize); j++){         
      if(j >= TreeH->GetEntries()){
	continue;
      }      
      
      TreeH->GetEntry(j); //Get the entries at j
      //Define the variables in the arrays
      dT[0] = dataT0[0];
      dT[1] = dataT1[0];
      dT[2] = dataT2[0];
      dT[3] = dataT3[0];
      dT[4] = dataT4[0];
      dT[5] = dataT5[0];
      dT[6] = dataT6[0];

      dA[0] = dataA0[0];
      dA[1] = dataA1[0];
      dA[2] = dataA2[0];
      dA[3] = dataA3[0];
      dA[4] = dataA4[0];
      dA[5] = dataA5[0];
      dA[6] = dataA6[0];
      
      double Tref = dataT7[0] * A;	 	 

      for(int k = 0; k < ng; k++){
	double time = dT[k] * A;              //Convert to picoseconds
	time -= (c1[k]/sqrt(dA[k])) + c0[k];  //Time walk correction
	time -= Tref;                           //Reference time
	fineHistList[k]->Fill(time);         //Fill histogram
      }
    }

    //Now we can make the fit
    double xCoarse = (double)i * timeLength / ((double)(TreeH->GetEntries()));

    //Now I can fit my coarses to my gausians
    for(int k = 0; k < ng; k++){
      TF1 * myfit = new TF1("myFit","gaus",minFine[k],maxFine[k]);   //Define Fit
      myfit->SetParameter(0,fineHistList[k]->GetMaximum());  //Find start parameters for a more acurate fit
      myfit->SetParameter(1,fineHistList[k]->GetMean());
      myfit->SetParameter(2,fineHistList[k]->GetStdDev());
      TFitResultPtr point = coarseHistList[k]->Fit(myfit,"qeSrn","",minFine[k],maxFine[k]); //Fit in the histogram range and use point to store the result
      int fitStatus = point;

      //Fill with resolution
      double time;
      double resolution;
      if(fitStatus == 0){
	time = point->Parameter(1);
	resolution = point->Parameter(2);
      }
      else{
	time = -1000;//fineHistList[k]->GetMean();
	resolution = -100;// fineHistList[k]->GetStdDev();
      }
      graphTimeList[k]->SetPoint(graphTimeList[k]->GetN(),xCoarse,time);
      graphResList[k]->SetPoint(graphResList[k]->GetN(),xCoarse,resolution);

      //Reset the coarse histogram
      delete fineHistList[k];
      coarseHistList[k]->Reset();
    }
  }
    
  cerr<<"Finished loop over TTree\n";
 
  inputFile->Close();


  //Write out
  outputFile->cd();  
  for (int k = 0; k < ng; k++){
    graphTimeList[k]->Write();
    graphResList[k]->Write();
  }
  outputFile->Close();
  
  cerr<< argv[2]<<" has been completed. \n\n\n";
  
  return 0;
}

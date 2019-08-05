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
int stepSize = 1000;

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
  

  //Now make a vector of graphs for the ADCs and time of flights
  vector<TGraphAsymmErrors*> graphList;

  TGraphAsymmErrors * graphA1 = new TGraphAsymmErrors();
  graphA1->SetName("ADC_1_time");
  graphList.push_back(graphA1);
  TGraphAsymmErrors * graphA2 = new TGraphAsymmErrors();
  graphA2->SetName("ADC_2_time");                      
  graphList.push_back(graphA2);
  TGraphAsymmErrors * graphA3 = new TGraphAsymmErrors();
  graphA3->SetName("ADC_3_time");                      
  graphList.push_back(graphA3);
  TGraphAsymmErrors * graphA4 = new TGraphAsymmErrors();
  graphA4->SetName("ADC_4_time");
  graphList.push_back(graphA4);
  TGraphAsymmErrors * graphA5 = new TGraphAsymmErrors();
  graphA5->SetName("ADC_5_time");                      
  graphList.push_back(graphA5);
  TGraphAsymmErrors * graphA6 = new TGraphAsymmErrors();
  graphA6->SetName("ADC_6_time");                                    
  graphList.push_back(graphA6);

  //Time of Flights
  TGraphAsymmErrors * graphT1 = new TGraphAsymmErrors();
  graphT1->SetName("TDC_1_time");                     
  graphList.push_back(graphT1);
  TGraphAsymmErrors * graphT2 = new TGraphAsymmErrors();
  graphT2->SetName("TDC_2_time");                                    
  graphList.push_back(graphT2);
  TGraphAsymmErrors * graphT3 = new TGraphAsymmErrors();
  graphT3->SetName("TDC_3_time");                     
  graphList.push_back(graphT3);
  TGraphAsymmErrors * graphT4 = new TGraphAsymmErrors();
  graphT4->SetName("TDC_4_time");                                    
  graphList.push_back(graphT4);
  TGraphAsymmErrors * graphT5 = new TGraphAsymmErrors();
  graphT5->SetName("TDC_5_time");                     
  graphList.push_back(graphT5);
  TGraphAsymmErrors * graphT6 = new TGraphAsymmErrors();
  graphT6->SetName("TDC_6_time");                                    
  graphList.push_back(graphT6);
  
  
  cerr<<"Histograms and Trees successfully created\n";
  
//Loop over TTree

  const double A = 0.025;

  for(int i = 0; i < (TreeH->GetEntries()); i += stepSize){
    
    //Display completed
    if((i%10000) == 0){
      cerr << (i*100.)/(TreeH->GetEntries()) <<"% completed events in the first loop \n";
    }

    //Use another loop to get the sums
    double sumT1 = 0;
    double sumT2 = 0;
    double sumT3 = 0;
    double sumT4 = 0;
    double sumT5 = 0;
    double sumT6 = 0;

    double sumA1 = 0;
    double sumA2 = 0;
    double sumA3 = 0;
    double sumA4 = 0;
    double sumA5 = 0;
    double sumA6 = 0;
    
    for(int j = i; j < (i+stepSize); j++){

      TreeH->GetEntry(j);

      //Convet TDC to units of picoseconds
      sumT1 += ( (dataT1[0] - dataT7[0]) * A ) - ( (c1[1]/sqrt(dataA1[0])) + c0[1] );
      sumT2 += ( (dataT2[0] - dataT7[0]) * A ) - ( (c1[2]/sqrt(dataA2[0])) + c0[2] );
      sumT3 += ( (dataT3[0] - dataT7[0]) * A ) - ( (c1[3]/sqrt(dataA3[0])) + c0[3] );
      sumT4 += ( (dataT4[0] - dataT7[0]) * A ) - ( (c1[4]/sqrt(dataA4[0])) + c0[4] );
      sumT5 += ( (dataT5[0] - dataT7[0]) * A ) - ( (c1[5]/sqrt(dataA5[0])) + c0[5] );
      sumT6 += ( (dataT6[0] - dataT7[0]) * A ) - ( (c1[6]/sqrt(dataA6[0])) + c0[6] );    

      sumA1 += dataA1[0];
      sumA2 += dataA2[0];
      sumA3 += dataA3[0];
      sumA4 += dataA4[0];
      sumA5 += dataA5[0];
      sumA6 += dataA6[0];
      
    }
    
    double x = ((double)i + 0.5) * timeLength / (double)(TreeH->GetEntries());
    
    graphA1 ->SetPoint(graphA1 ->GetN(),x,sumA1/stepSize);
    graphA2 ->SetPoint(graphA2 ->GetN(),x,sumA2/stepSize);
    graphA3 ->SetPoint(graphA3 ->GetN(),x,sumA3/stepSize);
    graphA4 ->SetPoint(graphA4 ->GetN(),x,sumA4/stepSize);
    graphA5 ->SetPoint(graphA5 ->GetN(),x,sumA5/stepSize);
    graphA6 ->SetPoint(graphA6 ->GetN(),x,sumA6/stepSize);
    					       
    graphT1 ->SetPoint(graphT1 ->GetN(),x,sumT1/stepSize);
    graphT2 ->SetPoint(graphT2 ->GetN(),x,sumT2/stepSize);
    graphT3 ->SetPoint(graphT3 ->GetN(),x,sumT3/stepSize);
    graphT4 ->SetPoint(graphT4 ->GetN(),x,sumT4/stepSize);
    graphT5 ->SetPoint(graphT5 ->GetN(),x,sumT5/stepSize);
    graphT6 ->SetPoint(graphT6 ->GetN(),x,sumT6/stepSize);
    
    }
     
  cerr<<"Finished loop over TTree\n";
 
  inputFile->Close();


  //Write out
  outputFile->cd();  
  for (int i = 0; i < graphList.size(); i++){graphList[i]->Write();}
  outputFile->Close();
  cerr<< argv[2]<<" has been completed. \n\n\n";
  
  return 0;
}


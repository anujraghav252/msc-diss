/*
 * Electron_Cut_Flow.C
 *
 * Description:
 * This ROOT macro performs a cut flow analysis on the DYtoLL_M50 Monte Carlo sample.
 * It selects events with exactly two tightly identified electrons and applies a series 
 * of kinematic and physics-based cuts to evaluate the number of events passing each stage.
 * The purpose is to study event selection efficiency and optimize cuts for signal purity.
 *
 * Cut Flow Stages:
 *  1. Exactly two tight electrons with opposite charge.
 *  2. Leading electron pt > 25 GeV, subleading pt > 20 GeV, both |η| < 2.5.
 *  3. Invariant mass of electron pair (m_ll) between 60 and 120 GeV.
 *  4. Projected MET < 20 GeV.
 *  5. Transverse momentum of dilepton system (pt_ll) < 40 GeV.
 *  6. Azimuthal angle difference (Δφ_ll) > 2.5.
 *
 * Input:
 *   - DYtoLL_M50.root (ROOT file containing a TTree named "Events")
 *
 * Output:
 *   - Printed cut flow summary showing the number of events surviving each cut stage.
 *
 * Author: Anuj Raghav
 * Date: 15-April-2025
 */




#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <iostream>
#include <cmath>
#include <TLorentzVector.h>

using namespace std;

void Electron_Cut_Flow(){
    TFile *file = TFile::Open("DYtoLL_M50.root");
    if (!file || file->IsZombie()) {
        cerr << "Error opening the ROOT file" << endl;
        return;
    }

    TTree *tree = (TTree*)file->Get("Events");
    if (!tree) {
        cerr << "Error getting the TTree from the ROOT file" << endl;
        return;
    }

    //Declareing variables
    UInt_t nElectron;
    Float_t Electron_pt[10];  
    Float_t Electron_eta[10];
    Float_t Electron_phi[10];
    Int_t Electron_pdgId[10];
    Bool_t Electron_mvaFall17V2Iso_WP90[10];
    Float_t projected_MET;

    //Setting branch addresses
    tree->SetBranchAddress("nElectron", &nElectron);
    tree->SetBranchAddress("Electron_pt", Electron_pt);
    tree->SetBranchAddress("Electron_eta", Electron_eta);
    tree->SetBranchAddress("Electron_phi", Electron_phi);
    tree->SetBranchAddress("Electron_pdgId", Electron_pdgId);
    tree->SetBranchAddress("Electron_mvaFall17V2Iso_WP90", Electron_mvaFall17V2Iso_WP90);
    tree->SetBranchAddress("projected_MET", &projected_MET);

  //setting counters
  int eventAfterstage1=0; //After nElectron ==2 and opposite charge
  int eventAfterstage2=0; //After electrons pass eta and pT cuts plus previous
  int eventAfterstage3=0; //After electrons pass mll cut and previous
  int eventAfterstage4=0; //After electrons pass pmet cut and previous
  int eventAfterstage5=0; //After electrons pass ptll cut and previous
  int eventAfterstage6=0; //After electrons pass dphill cut and previous
  Long64_t nEntries = tree->GetEntries();

  for (Long64_t i = 0; i < nEntries; i++) {
      tree->GetEntry(i);
  
      // Select tight electrons
      vector<int> tightElectrons;
      for (UInt_t j = 0; j < nElectron; j++) {
          if (Electron_mvaFall17V2Iso_WP90[j]) {
              tightElectrons.push_back(j);
          }
      }
  
      if (tightElectrons.size() != 2) continue;
  
      int i1 = tightElectrons[0];
      int i2 = tightElectrons[1];
  
      if (Electron_pdgId[i1] * Electron_pdgId[i2] < 0) {
          eventAfterstage1++;
  
          int lead = (Electron_pt[i1] > Electron_pt[i2]) ? i1 : i2;
          int sublead = (lead == i1) ? i2 : i1;
  
          if ((Electron_pt[lead] > 25) && (Electron_pt[sublead] > 20) &&
              (fabs(Electron_eta[lead]) < 2.5) && (fabs(Electron_eta[sublead]) < 2.5)) {
  
              eventAfterstage2++;
  
              TLorentzVector el1, el2;
              el1.SetPtEtaPhiM(Electron_pt[lead], Electron_eta[lead], Electron_phi[lead], 0.000511);
              el2.SetPtEtaPhiM(Electron_pt[sublead], Electron_eta[sublead], Electron_phi[sublead], 0.000511);
              TLorentzVector dilepton = el1 + el2;
  
              double mll = dilepton.M();
              double ptll = dilepton.Pt();
              double dphill = fabs(el1.DeltaPhi(el2));
  
              if (mll > 60 && mll < 120) {
                  eventAfterstage3++;
  
                  if (projected_MET < 25) {
                      eventAfterstage4++;
  
                      if (ptll < 40) {
                          eventAfterstage5++;
  
                          if (dphill > 2.5) {
                              eventAfterstage6++;
                          }
                      }
                  }
              }
          }
      }
  }
  
//Print output
cout << "\n\nCut Flow results for DYtoLL_M50.root:\n" << endl;
cout << "Total events in file "<<nEntries<<endl;
cout << "Events after nElectron == 2 and opposite charge: " << eventAfterstage1 << endl;
cout << "Events after |η| < 2.5 and leading pₜ > 25 GeV, subleading pₜ > 20 GeV: " << eventAfterstage2 << endl;
cout << "Events after dilepton mass 60 < mₗₗ < 120 GeV: " << eventAfterstage3 << endl;
cout << "Events after projected MET < 25 GeV: " << eventAfterstage4 << endl;
cout << "Events after pₜ^ll < 40 GeV: " << eventAfterstage5 << endl;
cout << "Events after |Δφ_ll| > 2.5: " << eventAfterstage6 << endl;
cout << "\nFinal events passing all cuts: " << eventAfterstage6 << endl;
cout << "\n\n";


delete file;
}

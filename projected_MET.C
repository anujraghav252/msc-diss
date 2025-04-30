/*
 * Macro: projected_MET()
 *
 * Description:
 * This ROOT macro calculates the projected MET and related angular variables 
 * for events in the "WWTo2L2Nu.root" file. It reads MET and electron data from 
 * the "Events" TTree and performs the following:
 * 
 * - Identifies the leading and subleading electrons based on pT.
 * - Computes deltaPhi (Δφ) between the MET vector and each of the two electrons.
 * - Determines the electron with the smallest |Δφ| (delta_phi_min).
 * - Calculates the projected MET using:
 *     projected_MET = MET * sin(|Δφ|), if |Δφ| < π/2;
 *     otherwise, projected_MET = MET.
 * - Adds and fills new branches:
 *     - delta_phi_1 (Δφ with leading electron)
 *     - delta_phi_2 (Δφ with subleading electron)
 *     - delta_phi_min (minimum |Δφ|)
 *     - projected_MET (modified MET for background suppression)
 * - Saves the updated tree back into the same ROOT file.
 *
 * This macro is useful for signal-background separation in events with 
 * multiple leptons, especially in analyses sensitive to the direction 
 * of missing energy.
 */


 #include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <cmath>
#include <iostream>

float deltaPhi(float phi1, float phi2) {
    float dphi = phi1 - phi2;
    while (dphi > M_PI) dphi -= 2 * M_PI;
    while (dphi <= -M_PI) dphi += 2 * M_PI;  // Adjusted to <= to handle -π properly
    return dphi;  // Returns signed value in [-π, π]
}

void projected_MET() {
    TFile *file = new TFile("WWTo2L2Nu.root", "UPDATE");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file 'WWTo2L2Nu.root'!" << std::endl;
        return;
    }

    TTree *tree = dynamic_cast<TTree*>(file->Get("Events"));
    if (!tree) {
        std::cerr << "Error: Tree 'Events' not found in file!" << std::endl;
        std::cerr << "Available objects in file:" << std::endl;
        file->ls();
        file->Close();
        delete file;
        return;
    }

    // Declare variables
    float PuppiMET_pt, PuppiMET_phi;
    UInt_t nElectron;
    const int MAX_ELECTRONS = 10;
    float Electron_pt[MAX_ELECTRONS], Electron_phi[MAX_ELECTRONS];

    // Check and set branch addresses
    bool branchesOk = true;
    if (tree->GetBranch("PuppiMET_pt")) {
        tree->SetBranchAddress("PuppiMET_pt", &PuppiMET_pt);
    } else {
        std::cerr << "Error: Branch 'PuppiMET_pt' not found!" << std::endl;
        branchesOk = false;
    }
    if (tree->GetBranch("PuppiMET_phi")) {
        tree->SetBranchAddress("PuppiMET_phi", &PuppiMET_phi);
    } else {
        std::cerr << "Error: Branch 'PuppiMET_phi' not found!" << std::endl;
        branchesOk = false;
    }
    if (tree->GetBranch("nElectron")) {
        tree->SetBranchAddress("nElectron", &nElectron);
    } else {
        std::cerr << "Error: Branch 'nElectron' not found!" << std::endl;
        branchesOk = false;
    }
    if (tree->GetBranch("Electron_pt")) {
        tree->SetBranchAddress("Electron_pt", Electron_pt);
    } else {
        std::cerr << "Error: Branch 'Electron_pt' not found!" << std::endl;
        branchesOk = false;
    }
    if (tree->GetBranch("Electron_phi")) {
        tree->SetBranchAddress("Electron_phi", Electron_phi);
    } else {
        std::cerr << "Error: Branch 'Electron_phi' not found!" << std::endl;
        branchesOk = false;
    }

    if (!branchesOk) {
        std::cerr << "One or more required branches missing!" << std::endl;
        file->Close();
        delete file;
        return;
    }

    // Create new branches
    float delta_phi_1 = 0, delta_phi_2 = 0, delta_phi_min = 0, projected_MET = 0;
    TBranch *b_delta_phi_1 = tree->Branch("delta_phi_1", &delta_phi_1, "delta_phi_1/F");
    TBranch *b_delta_phi_2 = tree->Branch("delta_phi_2", &delta_phi_2, "delta_phi_2/F");
    TBranch *b_delta_phi_min = tree->Branch("delta_phi_min", &delta_phi_min, "delta_phi_min/F");
    TBranch *b_projected_MET = tree->Branch("projected_MET", &projected_MET, "projected_MET/F");

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);
        
        delta_phi_1 = delta_phi_2 = delta_phi_min = projected_MET = 0;
        
        int leadIdx = -1, subleadIdx = -1;
        if (nElectron > MAX_ELECTRONS) {
            std::cerr << "Warning: Event " << i << " has " << nElectron 
                      << " electrons, truncating to " << MAX_ELECTRONS << std::endl;
            nElectron = MAX_ELECTRONS;
        }

        for (UInt_t j = 0; j < nElectron; j++) {
            if (leadIdx == -1 || Electron_pt[j] > Electron_pt[leadIdx]) {
                subleadIdx = leadIdx;
                leadIdx = j;
            } else if (subleadIdx == -1 || Electron_pt[j] > Electron_pt[subleadIdx]) {
                subleadIdx = j;
            }
        }

        if (leadIdx != -1) {
            delta_phi_1 = deltaPhi(PuppiMET_phi, Electron_phi[leadIdx]);
          // std::cout << "Event " << i << ": delta_phi_1 = " << delta_phi_1 << std::endl;
        }
        if (subleadIdx != -1) {
            delta_phi_2 = deltaPhi(PuppiMET_phi, Electron_phi[subleadIdx]);
            //std::cout << "Event " << i << ": delta_phi_2 = " << delta_phi_2 << std::endl;
        }
        
        if (leadIdx != -1 && subleadIdx != -1) {
            // Choose the delta_phi with smallest absolute value, preserving sign
            delta_phi_min = (std::abs(delta_phi_1) < std::abs(delta_phi_2)) ? delta_phi_1 : delta_phi_2;
          //  std::cout << "Event " << i << ": delta_phi_min = " << delta_phi_min << std::endl;
            // Use absolute value for projected MET calculation
            float abs_delta_phi_min = std::abs(delta_phi_min);
            projected_MET = (abs_delta_phi_min < M_PI/2) ? 
                           PuppiMET_pt * std::sin(abs_delta_phi_min) : PuppiMET_pt;
        } else if (leadIdx != -1) {
            delta_phi_min = delta_phi_1;
          //  std::cout << "Event " << i << ": delta_phi_min (single) = " << delta_phi_min << std::endl;
            projected_MET = PuppiMET_pt;
        }

        b_delta_phi_1->Fill();
        b_delta_phi_2->Fill();
        b_delta_phi_min->Fill();
        b_projected_MET->Fill();
    }

    tree->Write("", TObject::kOverwrite);
    file->Close();
    delete file;
}

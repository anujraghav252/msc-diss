/*
 * branch_extractor.C
 *
 * Description:
 * This ROOT macro extracts a subset of branches from multiple input ROOT files 
 * containing CMS Open Data or Monte Carlo simulation events. It efficiently reduces 
 * the size of the dataset by keeping only the necessary physics-related branches.
 *
 * Purpose:
 * - To streamline large datasets by keeping only relevant variables.
 * - To optimize memory usage and speed up further analyses.
 *
 * Functionality:
 * - Reads 61 ROOT files named DYtoLL1.root to DYtoLL61.root using a TChain.
 * - Disables all branches by default, then selectively enables only those listed in 
 *   'branches_to_keep'.
 * - Clones the filtered tree into a new output file `DYtoLL_ext1.root`.
 * - Uses small cache and basket sizes for memory efficiency.
 *
 * Output:
 * - A new ROOT file `DYtoLL_ext1.root` containing a slimmed-down version of the Events tree.
 *
 * Notes:
 * - Ensure all 61 input files are in the same directory and follow the naming convention.
 * - Update the file count or names in the loop if working with a different dataset.
 *
 * Author: Anuj Raghav
 * Date: 10-March-2025
 */


#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <iostream>
#include <vector>

void branch_extractor() {
    // Define branches to keep
    std::vector<std::string> branches_to_keep = {
        "run", "luminosityBlock", "event",
        "Electron_mvaFall17V2Iso_WP80", "Electron_mvaFall17V2Iso_WP90", "Electron_mvaFall17V2Iso_WPL",
        "Electron_mvaFall17V2noIso_WP80", "Electron_mvaFall17V2noIso_WP90", "Electron_mvaFall17V2noIso_WPL",
        "Electron_charge", "Electron_cutBased", "Electron_jetIdx", "Electron_pdgId", "Electron_photonIdx",
        "Electron_tightCharge", "Electron_phi", "Electron_pt", "Electron_r9", "Electron_scEtOverPt",
        "Electron_mass", "Electron_dxy", "Electron_dxyErr", "Electron_dz", "Electron_dzErr", "Electron_eCorr",
        "Electron_eInvMinusPInv", "Electron_energyErr", "Electron_eta", "Electron_hoe", "nElectron",
        "Electron_dEscaleDown", "Electron_dEscaleUp", "Electron_dEsigmaDown", "Electron_dEsigmaUp", "Electron_deltaEtaSC",
        "Electron_dr03EcalRecHitSumEt", "Electron_dr03HcalDepth1TowerSumEt", "Electron_dr03TkSumPt", "Electron_dr03TkSumPtHEEP",
        "CaloMET_phi", "CaloMET_pt", "GenMET_phi", "GenMET_pt", "MET_MetUnclustEnUpDeltaX", "MET_MetUnclustEnUpDeltaY",
        "MET_covXX", "MET_covXY", "MET_covYY", "MET_phi", "MET_pt", "MET_significance", "MET_sumEt", "MET_sumPtUnclustered",
        "Pileup_sumLOOT", "PuppiMET_phi", "PuppiMET_phiJERDown", "PuppiMET_phiJERUp", "PuppiMET_phiJESDown", "PuppiMET_phiJESUp",
        "PuppiMET_phiUnclusteredDown", "PuppiMET_phiUnclusteredUp", "PuppiMET_pt", "PuppiMET_ptJERDown", "PuppiMET_ptJERUp",
        "PuppiMET_ptJESDown", "PuppiMET_ptJESUp", "PuppiMET_ptUnclusteredDown", "PuppiMET_ptUnclusteredUp", "PuppiMET_sumEt",
        "Electron_genPartIdx", "Flag_goodVertices", "Pileup_nTrueInt", "Pileup_pudensity", "Pileup_gpudensity", "Pileup_nPU",
        "Pileup_sumEOOT", "Pileup_sumLOOT", "fixedGridRhoFastjetCentralChargedPileUp",
        "Jet_area", "Jet_btagCSVV2", "Jet_btagDeepB", "Jet_btagDeepCvB", "Jet_btagDeepCvL",
        "Jet_btagDeepFlavB", "Jet_btagDeepFlavCvB", "Jet_btagDeepFlavCvL", "Jet_btagDeepFlavQG",
        "Jet_eta", "Jet_phi", "Jet_pt", "Jet_mass", "Jet_electronIdx1", "Jet_electronIdx2", "Jet_jetId"
    };

    // Create a TChain for the specified ROOT files
    TChain *chain = new TChain("Events");
    for (int i = 1; i <= 61; ++i) {
        chain->Add(Form("DYtoLL%d.root", i));
    }

    // Check if chain has files
    if (chain->GetNtrees() == 0) {
        std::cerr << "Error: No valid input files found" << std::endl;
        delete chain;
        return;
    }

    // Disable all branches to save memory
    chain->SetBranchStatus("*", 0);

    // Enable only the required branches
    for (const auto &branch : branches_to_keep) {
        chain->SetBranchStatus(branch.c_str(), 1);
    }

    // Set small cache size (512 kB) for low memory usage
    chain->SetCacheSize(512 * 1024);

    // Create output file
    TFile *output_file = new TFile("DYtoLL_ext1.root", "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error: Could not create DYtoLL_ext1.root" << std::endl;
        delete chain;
        return;
    }

    // Switch to output file context
    output_file->cd();

    // Clone the chain into a new tree
    TTree *output_tree = chain->CloneTree(-1, "fast");
    if (!output_tree) {
        std::cerr << "Error: Failed to clone tree" << std::endl;
        delete chain;
        delete output_file;
        return;
    }

    // Set small basket size (8 kB) for memory efficiency
    output_tree->SetBasketSize("*", 8000);

    // Write and close the output file
    output_tree->Write("", TObject::kOverwrite);
    output_file->Close();

    // Clean up memory
    delete chain;
    delete output_file;

    std::cout << "Processing complete! Data from 61 input files saved in DYtoLL_ext1.root\n";
}

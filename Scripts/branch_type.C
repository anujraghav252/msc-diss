/*
 * Macro: branch_type()
 *
 * Description:
 * This ROOT macro opens a specified ROOT file (default: "DYtoLL1.root") 
 * and inspects the data types of a selected list of branches in the "Events" TTree.
 * 
 * Functionality:
 * - Opens the ROOT file and retrieves the "Events" tree.
 * - Checks for the existence of each branch in a predefined list.
 * - For each found branch, prints its data type using TLeaf::GetTypeName().
 * - Reports branches that are missing from the tree.
 * 
 * This utility is helpful for verifying data formats before analysis, 
 * debugging mismatched types, or generating branch-variable documentation.
 */
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <iostream>
#include <vector>

void branch_type(const char* filename = "DYtoLL1.root") {
    // Open the ROOT file
    TFile *file = TFile::Open(filename);
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open ROOT file!" << std::endl;
        return;
    }

    // Get the TTree
    TTree *tree = (TTree*)file->Get("Events");
    if (!tree) {
        std::cerr << "Error: Tree not found in file!" << std::endl;
        return;
    }



    // List of required branches
    std::vector<std::string> requiredBranches = {"nJet",
"Jet_area",
"Jet_btagCSVV2",
"Jet_btagDeepB",
"Jet_btagDeepCvB",
"Jet_btagDeepCvL",
"Jet_btagDeepFlavB",
"Jet_btagDeepFlavCvB"
,"Jet_btagDeepFlavCvL"
,"Jet_btagDeepFlavQG"
    };

    std::cout << "Checking Branch Types...\n";
    
    // Loop over required branches
    for (const auto& branchName : requiredBranches) {
        TBranch *branch = tree->GetBranch(branchName.c_str());
        if (!branch) {
            std::cout << "Branch: " << branchName << " not found!" << std::endl;
            continue;
        }

        // Get the leaf associated with the branch
        TLeaf *leaf = branch->GetLeaf(branchName.c_str());
        if (leaf) {
            std::cout << "Branch: " << branchName << ", Type: " << leaf->GetTypeName() << std::endl;
        } else {
            std::cout << "Branch: " << branchName << ", Type: Unknown" << std::endl;
        }
    }

    // Close the file
    file->Close();
}

# Data Analysis of the Drell–Yan Process in the CMS Experiment at the LHC

This repository contains the code developed for my final year dissertation project titled **"Data Analysis of the Drell–Yan Process in the CMS Experiment at the LHC"**.


## Project Overview

The primary goal of this project is to enhance the **signal and background purity** in the Drell–Yan process by applying a series of **sequential selection cuts** on data from the [CMS opendata](https://opendata.cern.ch/search?q&f=experiment%3ACMS&l=list&order=desc&p=1&s=10&sort=mostrecent) platform.

This repository contains C++ programs (in `.C` format) written using the **[ROOT](https://root.cern/)** framework, widely used in high-energy physics.

The code performs tasks such as:

- Calculating the **projected Missing Transverse Energy (MET)**
- Extracting and identifying **branch types** from ROOT TTrees
- Printing variable types for inspection and debugging
- Performing a **cut flow analysis**, showing how many events survive after each cut

## Requirements

- **ROOT** installed and properly configured
- Basic familiarity with **C++**
- A Linux-based environment

### Installing and Setting Up ROOT
Before installing ROOT, make sure the necessary dependencies are installed on your system. To do so, run the following commands on an Ubuntu-based Linux machine:

`sudo apt update`

`sudo apt install -y build-essential gcc g++ cmake python3-all \libffi-dev libssl-dev libxpm-dev libxft-dev libxext-dev \libpng-dev libjpeg-dev libgif-dev libtiff-dev libcurl4-openssl-dev \libboost-all-dev`

This installs the essential development tools and libraries that ROOT requires for compiling and running on your system.

After installing the required dependencies, you can proceed to install ROOT itself. Here’s how to configure the environment for ROOT:
First, download and install the ROOT source code. You can get the latest version from the official ROOT website, or use ‘wget‘ to download the ROOT tarball:

`wget https://root.cern.ch/download/root_v6.34.02.source.tar.gz`

Then, extract the tarball and follow the build instructions in the directory:

`tar -xvzf root_v6.34.02.source.tar.gz
cd root-6.34.02
mkdir build
cd build
cmake ..
make -j4
sudo make install`

Next, configure the ROOT environment by adding the following lines to your .bashrc file
(replace /path/to/root with the actual path to where ROOT is installed):

`export ROOTSYS=/path/to/root # Replace with actual ROOT install path
export PATH=$ROOTSYS/bin:$PATH
export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH
`
To apply the changes, run the following command to reload the environment:

`source ~/.bashrc`

To verify that ROOT has been successfully installed, run the following command in your terminal: `root`
If the installation was successful, you should see the following output: `root [0]`

This message confirms that ROOT is successfully installed and ready for use in analysis.


## Repository Contents

This repository contains ROOT macros (`.C` files) developed for the analysis of the Drell–Yan (DY) process in the CMS experiment:

### 1. `Electron_Cut_Flow.C`

- Performs a cut flow analysis on the DYtoLL_M50 Monte Carlo sample.
- Selects events with exactly two tightly identified electrons.
- Applies sequential kinematic and physics cuts:
  1. Exactly two tight electrons with opposite charge.
  2. Leading electron $p_T > 25\,\text{GeV}$, subleading $p_T > 20\,\text{GeV}$, both with $|\eta| < 2.5$.
  3. Invariant mass of electron pair between 60 and 120 GeV.
  4. Projected MET less than 20 GeV.
  5. Dilepton transverse momentum $p_T^{\ell\ell} < 40\,\text{GeV}$.
  6. Azimuthal angle difference $\Delta\phi_{\ell\ell} > 2.5$.
- Outputs the number of events surviving each cut stage.

---

### 2. `branch_extractor.C`

- Extracts a subset of branches from multiple ROOT files (`DYtoLL1.root` to `DYtoLL61.root`) containing CMS Open Data or Monte Carlo events.
- Disables all branches by default, selectively enables relevant physics branches.
- Creates a slimmed ROOT file (`DYtoLL_ext1.root`) with only needed variables.
- Optimizes memory and speeds up subsequent analyses.

---

### 3. `branch_type.C`

- Inspects the data types of specified branches in a ROOT file (default `DYtoLL1.root`).
- Checks for branch existence and prints their types.
- Useful for verifying data formats and debugging.

---

### 4. `projected_MET.C`

- Calculates projected Missing Transverse Energy (MET) and related angular variables for events in `WWTo2L2Nu.root`.
- Identifies leading and subleading electrons, computes delta phi angles between MET and electrons.
- Calculates projected MET based on these angles.
- Adds new branches to the tree: `delta_phi_1`, `delta_phi_2`, `delta_phi_min`, and `projected_MET`.
- Saves updated tree back into the ROOT file.
- Aids in signal-background separation in multilepton analyses.

---

### 5. `Superimposed_plots.C`
#
-  This ROOT macro (`superimposed_plots.C`) processes multiple CMS Open Data Monte Carlo samples to overlay **normalized kinematic distributions** of electron pairs (e.g., from Drell-Yan, tt̄, VV backgrounds).
-  Applies **tight electron selection** (`mvaFall17V2Iso_WP90`) and requires **exactly two opposite-charge electrons** to reconstruct dilepton quantities.
-  Plots distributions of `m_{ll}`, `p_T^{ll}`, projected MET, `Δϕ_{ll}`, and leading/subleading electron `p_T` and `η`, comparing across samples.
-  Saves each overlaid distribution as a `.png` file, labeled and styled for presentation.



---

Each macro is designed to be run using ROOT and contributes to improving the signal purity and background suppression in the Drell–Yan process analysis.


## Running a Macro in ROOT

You can run any `.C` macro interactively or in batch mode.

#### 1. Interactive Mode

Start ROOT in your terminal:

`
root 
.L Electron_Cut_Flow.C
Electron_Cut_Flow() `

#### 2. Batch Mode
Run a macro without opening the ROOT prompt:

`root -l -b -q Electron_Cut_Flow.C`

`-l` : Do not show the ROOT splash screen.

`-b` : Batch mode (no graphics).

`-q` : Exit ROOT after running the macro.




## Acknowledgments

This project was carried out as part of the final year undergraduate dissertation at [Department of Physics and Astrophysics, University of Delhi].  
I would like to express my sincere gratitude to:

- **[Dr. Arun Kumar](https://github.com/arunhep)** – for their continuous guidance and support.
- **The CMS Collaboration** – for making Open Data publicly accessible.
- **CERN** – for the ROOT framework and extensive documentation.

This work uses simulated datasets and tools provided under the [CMS Open Data initiative](https://cms-opendata.web.cern.ch/).

---

##  Contact

If you have questions, suggestions, or would like to collaborate, feel free to reach out:

- Email: anujraghav.physics@gmail.com
- GitHub: [anujraghav252](https://github.com/anujraghav252)
  

---

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








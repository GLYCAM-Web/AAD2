# Utilities for Automated Antibody Docking 2


**This code is changing rapidly. This file might not be up to date.**

You might also consider looking at the file merge-me-readme.md.


The scripts and binaries in this package facilitate the preparation of files to use as input for,
and the analysis of files resulting from, glycan-antibody docking experiments run using Vina-Carb.

## Workflow

1. Prepare the antibody (receptor).
   - Align the scfv region to optimize the geometry of the search volume.
   - Generate a pdbqt file to be used as input for docking.
2. Prepare the glycan (ligand).
   - Generate a pdbqt file to be used as input for docking.
   - Allow three flexibility options for the glycan: rigid, partially flexible, flexible.
3. Run the requested number of docking experiments.
   - Can be run in serial or parallel.
4. Analyze the docking results.

### Directory Structure

Working directory
   - User-supplied antibody and glycan pdb files
   - User-supplied configuration file for these utilities
   - Script-generated pdbqt files for antibody and glycan
   - User- or script-generated docking configuration file
   - Collected output from the docking replicas
   / Docking replica directories (one for each replica)
     - Symbolic links to the pdbqt and docking config files from the working directory
     - Output from the docking experiment
   

## Supported Computing Modes

The dependencies for this initial public-release version are very old. Because of this, the scripts assume 
that you might be employing Docker. It is possible to run these scripts entirely inside a Docker container.

The scripts also assume that you might be submitting batch requests to a cluster.

These are the supported modes.

- Serial docking on the local host. 

  This mode assumes that you are running in a computing environment that contains all the required
  dependencies, etc. The most likely manifestations of this mode are that you are running completely
  inside a container or virtual machine.

  By default, the docking software tries to use all the available processors. In this mode, the
  docking replicas will run in serial. 

- Batch submission to nodes on a cluster with head node available.

  Can be run with or without Docker.

  In this mode, the setup will be run on the head node. The docking replicas will be submitted to the 
  scheduler to be run on compute nodes. The analysis of the replicas will happen on the head node.

  The user should supply a wrapper script for the job submissions.

 
## List Of Executables

An 'X' indicates that the executable must be run in a special environment that contains very old compilers 
and other software. If you are using a Docker image for your special environment, these are the executables 
that must be run inside the image. The others can be run outside the container.

This information is useful if you want to run multiple docking replicas simultaneously. 

If you use the automated scripts, they will automatically choose the docker container or the local environment
based on each executable's needs.

For details on the special environment, see below.

This tree is out of date...
      ├── 0.configure
      ├── 1.prepare_antibody
      │   ├── align_antibody
    X │   │   ├── align 
    X │   └── generate_antibody_pdbqt.bash
      ├── 2.prepare_glycan
    X │   ├── generate_glycan_pdbqt.bash
    X │   ├── get_flexible_bond_info
      ├── 3.prepare_docking_directories
      │   └── prepare_AD_docking_directory.bash
      ├── 4.run_docking
      │   └── run.bash
      ├── 5.analyze_results
    X │   ├── cluster
    X │   ├── reorderpdb
    X │   └── write_output
      ├── Check_Status
    X ├── Compile
      ├── Install

If you run `Install.bash` with the option `nonspecial`, it will only copy files that do not require the 
special environment into the specified path. 

## The Special Environment

This code is very old and is only known to work with certain other software from a similar time. 

Older Dependencies:

* boost 1.41.0 
* gcc 4.4.7 
* mgltools x86 64 Linux 2 1.5.7
* vina-carb 1.0
* Python 2.7
* GEMS version?
* GMML version?
* GLYLIB version?

Dependencies that can be more recent:

* gnuplot
* bash
* make


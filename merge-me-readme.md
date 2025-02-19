# Automated Antibody Docking 2

This version of Automated Antibody Docking is very similar to the original. It has been changed only
to the extent needed to make it more portable and easier to run in parallel. It still relies on the 
very old compilers and ancillary code that the original used.

Because of the older code, this version is made to be easily run inside a docker container.
Here is the GitHub repo containing helper scripts for using Docker.

    AAD2_Docker  

If you want to try to run it outside docker, the requirements can be found in this repo:

    AAD2_Dependencies

This repo is also a sub-module to `AAD2_Docker`.

## Changes from the original AAD

The main difference is that the various steps used in the course of the AAD have been decoupled - that
is, separated out into individual programs - so that they are mostly independent of each other. Doing 
this simplifies many tasks related to maintaining and porting the code. Notably, it also facilitates 
running many docking replicas in parallel. 

The first version ran all docking experiments in serial and did the analysis at the end (and there were 
reasons for that). You can still run it all in serial if you like.

## Basic Workflow

This workflow assumes that you have installed the executables into a place on your PATH. If not,
the full paths are also indicated below.

1. Make a directory for the job's input and output.
2. Optionally run `AAD2_Setup <work-dir>`. This will write example config files to the directory and 
   leave a list of requirements in a log file.
3. Ensure that the working directory contains all the things it needs.
4. Run `AD_All <work-dir>` if you want to run everything at once.
   - If you prefer to run things individually, run these:
     1. `AD_Evaluate <work-dir>`
     2. `AD_Run <work-dir>`
     3. `AD_Analyze <work-dir>`
   - For even more granularity, check out the contents of these directories:
     - `0.configure`
     - `1.prepare_antibody`
     - `2.prepare_glycan`
     - `3.prepare_docking_directories`
     - `4.run_docking`
     - `5.analyze_results`
     - `6.check_status`

## Compiling

Running:

    ./Compile

will compile the programs that need to be compiled.

Note that the code is very old. Please see discussions regarding dependencies.

## Installing

You do not need to install this software. You can just run the scripts from within this directory.
But, if you prefer to have them in a more convenient location, run:

    ./Install.bash <convenient location>

This will put the executable scripts into the 'bin' subdirectory in 'convenient location'.


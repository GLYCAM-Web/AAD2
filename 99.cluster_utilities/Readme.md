# Cluster Utilities for AAD2

**This code is changing rapidly. These docs may be incomplete or out of date.** 

These utilities should help you to use AAD2 on a cluster. It is possible that you will need to use these
as inspiration for your own scripts rather than being able to use them as-is. This will certainly be the
case if your cluster uses a scheduler other than Slurm.

Specifically, they manage the process of running some parts of the code on the head node, then sending 
some to compute nodes, and then running more on the head node.

They are needed due to the dependence of AAD2 on a very old operating environment. It is very unlikely 
that the IT folks in charge of your cluster's head node will be comfy with adding the environmental 
requirements to the head node. But, it is possible that they might install a few nodes (virtual or 
otherwise) for you to use. 

If you are able to use Docker with your cluster, there are also utils for that. What you will find here
are wrappers for the actual scripts that can be found in the 
[`AAD2_Docker` repo on GitHub](https://github.com/GLYCAM-Web/AAD2_Docker).

## Workflows

Some parts of the code can run almost anywhere. Other parts must run in a specialized environment. That
environment might be in the form of virtualization (Docker, virtual machine, etc.) or in the form of
a separate machine or cluster (on metal) that you can use.

The 'noS' and 'onlyS' variables at the top of the AAD2/Install script list, respectively, the actions
that can be run anywhere (noS) and those that must be in the special environment (onlyS). 

Note that you can choose to only install the 'noS' variants.

Generally, these are the steps you will use on a cluster. Steps that start with '(S)' must be run in 
the special environment required by AAD2.

 1. Set up the working directory: Copy in files; run `AD_Setup` and `AD_Validate`.
 2. (S) Evaluate the input files for compatibility and transform them in to docking input using `AD_Evaluate`.
 3. Prepare the subdirectories for the docking job replicas using `prepare_AD_docking_directory`.
 4. Submit the docking jobs to the scheduler using `start_all_docking_jobs`. Note next!
 5. (S) Run the docking job replicas. The nodes where the docking jobs run must have the special 
    environment available. 
    * If these run on metal, use `run_AD_docking`.
    * If they run in a docker container that is spun up on the node, use `run_docking_on_node.bash`. See
      the `AAD2_Docker` repository for more info.
    * See also below about chaining steps 5, 6, and 7.
 6. Monitor the jobs and trigger analysis when complete. You can use `check_AD_status`, but see below.
 7. (S) Analyze the results of the job using `AD_Analyze`. Also, see below.

When set up correctly, steps 5, 6, and 7 can proceed in an automated manner.

 1. The script `Submit_and_Spawn_Monitor` will submit the jobs and then spawn `Monitor_and_Analyze` and
    then it will exit (leaving the latter still running).
 2. `Monitor_and_Analyze` will monitor the system every 5 seconds (configurable) until all the replicas
    have completed. At that point, it will run `AD_Analyze`.

## CLI Option

`Run_AAD2_from_CLI.bash` will run the entire workflow from start to finish from the command line.

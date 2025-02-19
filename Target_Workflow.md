# Target Workflow for AD2 Utils

Rationale for this:  The step that analyzes output from multiple runs must be a 'meta' workflow that
exists outside of the multiple docking runs. So, the actual docking will be a little annoying to write.

The user will need to provide a configuration file. Contents below.

 1. Get from the 'user' the names of the antibody pdb and the glycan pdb. Also get the number of jobs.
 2. Set up the parent directory for the jobs with the two pdbs included and named properly.
 3. !! Run the setup parts to make the two pdbqt files and the config file.
 4. Generate subdirectories with symbolic links to the required parent info.
 5. Place inside each subdirectory a script that will run a single docking job.
    1. The script run in each subdirectory will always have the same name. This will be advertised in the documentation.
    2. !! This should be a single script that does everything AD needs.
 6. If this will be submitted to a cluster, get the user to provide a script that will submit the AD2 
    script as a job to the scheduler. 
    1. This script should save any needed info such as job names, return status, etc.
    2. If the submission fails, the script should tell the AD2 runner script.
    3. The batch script should need only to do batch stuff and then call this script.
    4. The user should generate the batch script to be placed in each directory and to tell AD2 the name of the script.
    5. It should be copied in from somewhere, and AD2 should not need to alter it.
    6. It can be a wrapper script that generates custom parts in a runtime-generated batch script.
 7. The AD2 script cd's to each directory and runs the script, saving output to a log.
 8. The AD2 script will only be able to check the subdirectories to see if the job has started or not and
    whether it has ended and if it has ended, whether it seemed to work.
 9. !! The AD2 runner script will run the analysis scripts once all docking runs complete.
10. If one of them fails? Ugh....  Maybe Nada for now. Or whatever R sez to do.

Presence of '!!' indicates that this code must run in a special environment.

Three modes are likely in our setup:

1. Everything runs in serial inside a docker container. This will mostly be used for testing and such.
2. All parts that require a special environment get submitted to a node that contains the correct environment for AD. 
   This node might be a docker container, as it will be in the DevEnv.
3. All parts that require a special environment will:
   - For setup and analysis, run in a docker container on the head node.
   - For the docking, run in a docker container on a compute node (accessed via batch submission).
   - This will be the mode on a compute cluster. The separation is so that the easy/fast 'evaluation' (generation of 
     the pdbqt files) can happen without waiting in queue.

## Configuration File

Contents:

- original antibody file name
- original glycan file name
- number of docking runs
- computing mode - see 1, 2 & 3 above.
- if mode 2 or 3:
  - name and location of script to copy into each docking directory and run on a head node
- if mode 2: name and location of script to submit the simple stuff to the queue



#!/usr/bin/env bash

## This script should be run from inside the working directory
##
## The following files should be present in the directory
##
##
##    User's antibody PDB file
##    User's glycan PDB file
##    glycan_ring_atoms.txt
##    ad2config - AAD2 config file
##    The script that should be run to submit jobs
##    If needed, a local configuration file for the job submission script

export WD="$(pwd)"
source ad2config

cd ${AAD2_DOCKER_HOME}
COMMAND="bash bin/run_aad2_command.bash ${WD} AD_Evaluate"
echo "The cwd is:
$(pwd)
The script will run this command:
${COMMAND}"
eval ${COMMAND}
#
# for website workflows, copy in the updated ad2config with flexibility and replicas info
#

cd ${WD}
# The following will spawn a monitoring script and run AD_Analyze once all jobs complete
COMMAND="submit_and_spawn_monitor"
echo "The cwd is:
$(pwd) 
The script will run this command:
${COMMAND}"
eval ${COMMAND} 


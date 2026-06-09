#!/usr/bin/env bash

# The script to be submitted via sbatch

source ad2config   
echo "sourced ad2config"
source ${AD2_Docking_Local_Script} 
echo "sourced AD2_Docking_Local_Script (${AD2_Docking_Local_Script})"

#SBATCH --chdir=${WORKDIR}
#SBATCH --partition=${PARTITION}
#SBATCH --error=%x-%A.err
#SBATCH --output=%x-%A.out
#SBATCH --get-user-env
#SBATCH --job-name=ad-${CONTAINER_NAME_PREFIX}
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=${DOCKING_REPLICA_BATCH_CPUS}

bash ${CLUSTER_EXE_NAME} >> ${WJOB_LOG}


#!/usr/bin/env bash

# Submits the docking job in the **current directory** to a slurm cluster.
# Requires well-populated ad2config and ad2dockerconfig files.

##
# This script assumes that paths on the compute nodes match those on the head node.
##

#
# ad2config following contains
#    Log_File
#    AAD2_CLI_BIN_PATH
#    AD2_Docking_Local_Script
source ad2config   

echo "sourced ad2config"

if [ -z "${Log_File}" ] ; then
	Log_File='ad2.log'
fi

#
# AD2_Docking_Local_Script contains
#    pUUID
#    DOCKING_REPLICA_BATCH_CPUS # can differ from AD2_Docking_CPUS
#    SUBMIT_FILE_NAME
#    CLUSTER_EXE_NAME
#    AAD2_BIN_PATH # can be overridden if need be
#    AAD2_DOCKER_HOME # can be overridden if need be
# The likely name:  gwconfig # GLYCAM-Web configuration file
echo "checking for local script"
if [ -z "${AD2_Docking_Local_Script}" ] ; then
	echo "ERROR: AD2_Docking_Local_Script must be defined in ad2config. Exiting" 2>&1 > ${Log_File}
	exit 1
fi
echo "about to source local script"
source ${AD2_Docking_Local_Script} 
##echo "ERROR: cannot source ${AD2_Docking_Local_Script}. Exiting." 2>&1 > ${Log_File} && exit 1
echo "sourced ${AD2_Docking_Local_Script}"

if [ -z "${CLUSTER_EXE_NAME}" ] ; then
	echo "ERROR: CLUSTER_EXE_NAME must be defined in ${AD2_Docking_Local_Script}. Exiting" 2>&1 > ${Log_File}
	exit 1
fi
echo "the cluster exe name is: ${CLUSTER_EXE_NAME}"

if [ -z "${DOCKING_REPLICA_JOB_LOG}" ] ; then
	JOB_LOG='ad2_job.log'
else
	JOB_LOG="${DOCKING_REPLICA_JOB_LOG}"
fi

WORKDIR="$(pwd)"
tmpdirname="$(basename ${WORKDIR})"
repNum="${tmpdirname#"r"}"
CONTAINER_NAME_PREFIX="${pUUID:0:5}-${repNum}"
WJOB_LOG="${WORKDIR}/${JOB_LOG}"

### One day, not having separate paths on the cluster will be a problem.
### For now, not writing for it
#if [ -z "${CLUSTER_WORKDIR}" ] ; then
	#CLUSTER_WORKDIR="${WORKDIR}"
#fi

# Script to be run by the submission script
runMe="""#!/usr/bin/env bash
source ad2config
source ${AD2_Docking_Local_Script}

cd ${WORKDIR} 
export PATH=${AAD2_CLI_BIN_PATH}/bin:\$PATH
echo \"The local computing host is: \$(hostname)\" >> ${WJOB_LOG}
export AAD2_DOCKER_HOME
module load docker >> ${WJOB_LOG}
module load runc >> ${WJOB_LOG}
ensure_image_is_present >> ${WJOB_LOG}
cd ${AAD2_DOCKER_HOME} 
export CONTAINER_NAME_PREFIX=${CONTAINER_NAME_PREFIX}
bash bin/run_docking_on_node.bash ${WORKDIR}  >> ${WJOB_LOG}
"""
echo "${runMe}" > "${CLUSTER_EXE_NAME}"

# Script to submit
submitMe="""#!/usr/bin/env bash
#SBATCH --chdir=${WORKDIR}
#SBATCH --error=%x-%A.err
#SBATCH --output=%x-%A.out
#SBATCH --get-user-env
#SBATCH --job-name=ad-${CONTAINER_NAME_PREFIX}
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=${DOCKING_REPLICA_BATCH_CPUS}

bash ${CLUSTER_EXE_NAME} >> ${WJOB_LOG}
"""
echo "===== Begin Submission ====================================" >> "${WJOB_LOG}"
echo "About to submit docking run for working directory: ${WORKDIR}" >> "${WJOB_LOG}"

echo "Writing submission file to ${SUBMIT_FILE_NAME} and submitting." >> "${WJOB_LOG}"
echo "${submitMe}" > "${SUBMIT_FILE_NAME}"
sleep 1
sbatch ${SUBMIT_FILE_NAME} >> "${JOB_LOG}"
returnvalue=$?
if [ "${returnvalue}" != "0" ] ; then
    echo "Something went wrong submitting the job." >> "${WJOB_LOG}"
    echo "The return value was: ${returnvalue}" >> "${WJOB_LOG}"
fi
echo "===== End Submission ======================================" >> "${WJOB_LOG}"


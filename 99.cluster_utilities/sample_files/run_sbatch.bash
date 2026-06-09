#!/usr/bin/env bash

# Submits the docking job in the **current directory** to a slurm cluster.

source ad2config
echo "sourced ad2config"
source ${AD2_Docking_Local_Script}
echo "sourced AD2_Docking_Local_Script (${AD2_Docking_Local_Script})"

# Script to submit
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


#!/usr/bin/env bash
source ad2config   
echo "sourced ad2config"
source ${AD2_Docking_Local_Script} 
echo "sourced AD2_Docking_Local_Script (${AD2_Docking_Local_Script})"

cd ${WORKDIR} 
export PATH=${AAD2_CLI_BIN_PATH}/bin:\$PATH
echo \"The local computing host is: \$(hostname)\" >> ${WJOB_LOG}
export AAD2_DOCKER_HOME
# module load docker >> ${WJOB_LOG}  ## Will not be needed on all systems
ensure_image_is_present >> ${WJOB_LOG} # see PATH export just above for location
cd ${AAD2_DOCKER_HOME} 
export CONTAINER_NAME_PREFIX=${CONTAINER_NAME_PREFIX}
bash bin/run_docking_on_node.bash ${WORKDIR}  >> ${WJOB_LOG}


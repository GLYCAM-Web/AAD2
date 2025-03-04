#!/usr/bin/env bash
source ad2cliconfig

export WD=${WD}
if [ ! -d "${WD}" ] ; then
	mkdir ${WD} || echo "cannot find or make working directory." || exit 1
fi

cp \
  ${USER_AB_FILEPATH} \
  ${USER_G_FILEPATH} \
  ${AD2CFG_FILEPATH}  \
  ${AD2DOCKERCFG_FILEPATH}  \
  ${LOCAL_COMPUTECFG_FILEPATH} \
  ${COMPUTE_SUBMIT_SCRIPT_FILEPATH} \
  ${WD}

cd ${WD}

# This needs to happen somewhere...
# if [[ "$(tail -1 ${userGpdb})" != *"END"* ]] ; then echo "END" > ${userGpdb} ; fi
#
# For the moment, fake this, but when I fix detect sugars, this is where it goes
# ( cd $GEMSHOME/gmml/tests && ${DETECT_SUGARS} ${WD}/${userGpdb} > ${WD}/glycan_rings.txt )
#
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
# will run AD_Analyze once all jobs complete
COMMAND="Submit_and_Spawn_Monitor"
echo "The cwd is:
$(pwd)
The script will run this command:
${COMMAND}"
eval ${COMMAND}


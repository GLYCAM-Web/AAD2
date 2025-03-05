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

# This should already be done
#cp glycan_ring_atoms.txt.save ${WD}/glycan_ring_atoms.txt
#
# This should already be done
# Ensure that the glycan pdb used as an input contains an 'END' card at the bottom of the file.
#if [[ "$(tail -1 ${USER_G_FILEPATH})" != *"END"* ]] ; then echo "END" >> ${USER_G_FILEPATH} ; fi
#
# This should already be done
# Automatically generate the glycan rings info file. requires GEMSHOME and a compiled detect_sugars in gmml/tests
#( cd $GEMSHOME/gmml/tests && ${DETECT_SUGARS} ${WD}/${USER_G_FILEPATH} > ${WD}/glycan_rings.txt )

cd ${WD}

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


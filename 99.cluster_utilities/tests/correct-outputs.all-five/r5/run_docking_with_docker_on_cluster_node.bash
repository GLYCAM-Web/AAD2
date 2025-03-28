#!/usr/bin/env bash
source ad2config
source gwconfig

cd /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r5 
export PATH=/programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/bin/bin:$PATH
echo "The local computing host is: $(hostname)" >> /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r5/ad2_job.log
export AAD2_DOCKER_HOME
module load docker >> /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r5/ad2_job.log
module load runc >> /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r5/ad2_job.log
ensure_image_is_present >> /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r5/ad2_job.log
cd /programs/website_aad2/swarmtest/AAD2_Docker 
export CONTAINER_NAME_PREFIX=abcde-5
bash bin/run_docking_on_node.bash /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r5  >> /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r5/ad2_job.log


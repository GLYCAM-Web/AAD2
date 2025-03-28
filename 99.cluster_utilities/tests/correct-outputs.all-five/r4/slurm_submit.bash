#!/usr/bin/env bash
#SBATCH --chdir=/programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r4
#SBATCH --error=%x-%A.err
#SBATCH --output=%x-%A.out
#SBATCH --get-user-env
#SBATCH --job-name=ad-abcde-4
#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=16

bash run_docking_with_docker_on_cluster_node.bash >> /programs/website_aad2/swarmtest/AAD2_Docker/image/AAD2/99.cluster_utilities/tests/outputs.all-five/r4/ad2_job.log


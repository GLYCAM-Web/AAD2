# AAD2 Tests For Cluster Utilities

These tests assume that you are on a Slurm cluster and that you can use Docker on the head node and on
the compute nodes.

## Running the test

1  Make a file called `Local_Settings.bash`.  

AAD2_IMAGE_FILE_PATH=\"${AAD2_IMAGE_FILE_PATH}\"
Image=\"${Image}\"
AAD2_DOCKER_HOME=\"${AAD2_DOCKER_HOME}\"
AAD2_CLI_BIN_PATH=\"${AAD2_CLI_BIN_PATH}\"

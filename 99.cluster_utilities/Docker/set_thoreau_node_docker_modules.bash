#!/usr/bin/env bash

# Setup needed on the Thoreau compute nodes in order to run docker there.

# So far, we only need to load two modules
module load iptables/1.8.7
module load docker/24.0.7 

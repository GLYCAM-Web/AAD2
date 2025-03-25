# AD2 Utils Configuration

The user should generate a configuration file and place it in the top-level docking directory.

The configuration file should be named `ad2config`.

A sample file is included in this directory.

## Configuration Options: 

General Requirements:

- Files should be in the working directory or their names should contain path information.
- The scripts expect to be executed from the working directory.
- Configuration options should be expressed as Key=Value with no space between Key and Value.

Configuration options:

`MGLToolsPckgs_Location` - The path to the MGLToolsPckgs directory (including that directory).

`Vina_Carb_Source_Location` - Path to the VC source code.

`Python_2_7_Location` - Path to the directory containing python.h for python version 2.7.

`Antibody_File_Name` - The name of the pdb file containing the antibody (receptor). 

`Glycan_File_Name` - The name of the pdb file containing the glycan (ligand).

`Number_of_Replicas` - The number of docking replicas to run. Default is "1".

`Glycan_Flexibility` - 

  - "Rigid" - Corresponds to option 1 in the AD code.

  - "Partial" - Corresponds to option 2 in the AD code.

  - "Flexible" - Corresponds to option 3 in the AD code.

`Computing_Mode` - 

  - `Local_Host_Serial` - Run everything serially on the current machine. This is the default.

  - `Batch` - Submit some or all of the tasks to a cluster. see `AD2_Batch_Setup` and `AD2_Batch_Analysis`

`Use_Docker` - Set to "True" if you will be running tasks inside a Docker container. Default is "False".

`Docking_Batch_Script` - The script to use for submitting the docking replicas. Default is "None".

`AD2_Batch_Setup` - The script to use if the setup needs to be submitted to a cluster. Default is "None".

`AD2_Batch_Analysis` - The script to use if the analysis needs to be submitted to a cluster. Default is "None".

`AD2_Random_Seeds` - An array (BASH array) of random number seeds to use. 
The array should contain `Number_of_Replicas` of them.
Here is an example of a properly formatted array. Note that the random number seeds can be negative.
```
AD2_Random_Seeds=(
        [1]="1770199568"
        [2]="-342100090"
        [3]="1840569422"
        [4]="-271726951"
        [5]="1910945648"
)
```


## Docker Configuration

If you will be starting Docker containers during the workflow, these need to be set.

`Image` - The image, with tag, to use.  Format: "myImage:myTag". The default is "None".


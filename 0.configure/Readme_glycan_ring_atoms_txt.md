# The Glycan Ring Atoms File

There must be a file in the directory that contains information about the cycles in the glycan.

You can make the file by hand. It is also possible to have it made automatically.  See more on 
that below.

The format is as follows:

1. Any residue containing 6 or more atoms must be included in the file.
2. Each record begins with the word ATOM and ends with the word END.
3. Between ATOM and END must be the names of the ring atoms, one per line.
4. The records must appear in the order the residues appear in the glycan pdb file.

There is an example file in:

    0.configure/sample_files

This file corresponds to the glycan in:

    DManpa1-8DNeup5Acb2-4DFrufa2-OH_structure.pdb

To generate the file automatically:

    git clone -b feature_cycleDetection https://github.com/GLYCAM-Web/gmml gmml
    cd gmml
    ./make.sh
    cd tests
    GMML_ROOT_DIR=$(git rev-parse --show-toplevel)
    g++ -std=c++17 -I "${GMML_ROOT_DIR}" -L"${GMML_ROOT_DIR}"/bin/ -Wl,-rpath,"${GMML_ROOT_DIR}"/bin/ tests/007.detectSugars.cc -lgmml -lstdc++fs -pthread -o detect_sugars

After that, you can use, from the tests directory:

    ./detect_sugars /path/to/your/glycan.pdb

Please check that the output matches the glycan. 

#include <glylib.h>

typedef struct {
    int resNum; ///< The number assigned to the residue inside the file
    int nA; ///< Number of atoms in the ring
    char **Name;///< Names of the atoms in the ring
    coord_3D *x; ///< 3D coordinates for each ring atom
} ring_set; ///< A set of info about a ring

void get_rmsd(char *reffile,char* allmodels,char *ringatomsfile){

fileset Cset,Pset,Oset;
assembly *Reference_PDB;
int i=0;

Cset.N=strdup(reffile);
Reference_PDB=load_pdb(Cset.N);
// Ensure that there is only one molecule in the Reference_PDB
if(Reference_PDB[0].nm>1){
    printf("ERROR in get_rmsd: The Reference PDB file contains more than one molecule.\n");
    printf("      The reference file name is: %s \n",reffile);
}
//printf("just loaded %s The number of residues in this molecule are %d\n",reffile,Reference_PDB[0].m[0][0].nr);

fileslurp Islurp;
fileset Iset;
Iset.N=strdup(ringatomsfile);
Islurp=slurp_file(Iset);
coord_3D **ring;
ring=(coord_3D**)calloc(6,sizeof(coord_3D*));

ring_set *Reference_Set;
int nRings=0;
//printf("Counting the number of residues with rings\n");
i=0;
while(i<Islurp.n){
        if(strstr(Islurp.L[i],"RES") != NULL){ 
        i++; 
        nRings++;
    }
    i++;
}
Reference_Set = (ring_set*)calloc(nRings,sizeof(ring_set));
//printf("Counting the number of atoms in each of the %d ring(s)\n", nRings);
i=0;
int nr=0;
int nAtoms=0; // interim couter for atoms in each ring
int nTotalRingAtoms=0; // total atoms in all the considered rings
while(i<Islurp.n){
    nAtoms=0;
        if(strstr(Islurp.L[i],"RES") != NULL){ 
            i++; 
            while(strstr(Islurp.L[i],"END") == NULL){
                i++;
                nAtoms++;
            }
            Reference_Set[nr].nA=nAtoms;
            Reference_Set[nr].Name=(char**)calloc(nAtoms,sizeof(char*));
            Reference_Set[nr].x=(coord_3D*)calloc(nAtoms,sizeof(coord_3D)); ///< 3D coordinates for each ring atom
	    nTotalRingAtoms+=Reference_Set[nr].nA; ///< Set total atoms in all rings for use near the end
            nr++;
        }
    i++;
}

i=0;
nr=0;
int na=0;
char dum[50];
while(i<Islurp.n){
    if(strstr(Islurp.L[i],"RES") != NULL){
        //printf("Islurp.L[%d] is >>>%s<<<\n",i,Islurp.L[i]);
        sscanf(Islurp.L[i],"%s %d",dum,&Reference_Set[nr].resNum);
        i++;
        na=0;
        while(strstr(Islurp.L[i],"END") == NULL){
            //printf("Islurp.L[%d] is >>>%s<<<\n",i,Islurp.L[i]);
            sscanf(Islurp.L[i],"%s",dum);
            Reference_Set[nr].Name[na]=strdup(dum);
            //printf("dum is >%s< and Reference_Set[%d].Name[%d] is >>>%s<<<\n",dum,nr,na, Reference_Set[nr].Name[na]);
            i++;
            na++;
        }
        nr++;
    }
i++;
}

// Ensure that no two residues in the glycan have the same residue number
int nrc=0; // comparison ring counter
for(nr=0;nr<nRings;nr++){
    for(nrc=nr+1; nrc<nRings; nrc++){ 
        if(Reference_Set[nr].resNum == Reference_Set[nrc].resNum){
            printf("ERROR: In get_rmsd, duplicate residue numbers are not allowed in the glycan.\n");
            printf("       Residues with indexes %d and %d are both called number %d.\n",nr, nrc, Reference_Set[nrc].resNum);
            exit(1);
        }
    }
}


// Copy the coordinates from the Reference_PDB into the Reference_Set
int      nr_pdb=0; // counter for residues in the pdb file
int      na_pdb=0; // counter for atoms in residues in the pdb file
atom *this_pdb_atom; // an atom pointer to make the code more readable
residue *this_pdb_res; // a residue pointer to make the code more readable
int      same=1; // 1 is false here
for ( nr=0 ; nr<nRings ; nr ++ ){
    for ( nr_pdb=0 ; nr_pdb<Reference_PDB[0].m[0][0].nr ; nr_pdb++){
        this_pdb_res=&Reference_PDB[0].m[0][0].r[nr_pdb];
        if ( this_pdb_res[0].n == Reference_Set[nr].resNum ) {
            for ( na=0 ; na<Reference_Set[nr].nA ; na++ ) {
		for ( na_pdb=0 ; na_pdb<this_pdb_res[0].na ; na_pdb++ ) { 
	            this_pdb_atom=&this_pdb_res[0].a[na_pdb]; 
		    same = strcmp( Reference_Set[nr].Name[na] , this_pdb_atom[0].N ); 
		    if ( same == 0 ) {
                        Reference_Set[nr].x[na] = this_pdb_atom[0].x ;
			break;
		    }
		}
            }
        }
    }
}
// Uncomment the next block to test that the coordinates copied correctly
// The coords typically come from a file with a name format like this:  model1.pdb
/** /
for ( nr=0 ; nr<nRings ; nr ++ ){
    printf("RESIDUE NUMBER: %d coordinates\n",Reference_Set[nr].resNum);
    for ( na=0 ; na<Reference_Set[nr].nA ; na++ ) {
	printf("ATOM %s : %4.2lf  %4.2lf  %4.2lf\n",
            Reference_Set[nr].Name[na],
            Reference_Set[nr].x[na].i,
            Reference_Set[nr].x[na].j,
            Reference_Set[nr].x[na].k);
    }
}
/ **/

// Declare and initialize the variables and structures for the set of comparison structures
// These structures come from the file referenced by "allmodels" in the input.
Pset.N=strdup(allmodels);
char *allfile;
allfile=(char*)calloc(50,sizeof(char));
sprintf(allfile,"%s",Pset.N);
assembly *Model_PDB;// The assemply that stores each comparison structure (model) as a molecule
Model_PDB=load_pdb(allfile); 
//printf("the number of molecules in this assembly are %d\n",Model_PDB[0].nm); // uncomment to test the load_pdb
ring_set *Model_Set;  // This will hold only the ring coordinates for each comparison structure
Model_Set = (ring_set*)calloc(nRings,sizeof(ring_set));
for ( nr=0 ; nr<nRings ; nr++ ) {
    Model_Set[nr].resNum = Reference_Set[nr].resNum;
    Model_Set[nr].nA = Reference_Set[nr].nA;
    Model_Set[nr].Name = (char**) calloc(Reference_Set[nr].nA,sizeof(char*));
    Model_Set[nr].x = (coord_3D*) calloc(Reference_Set[nr].nA,sizeof(coord_3D));
    for ( na=0 ; na<Reference_Set[nr].nA ; na++ ){
        Model_Set[nr].Name[na] = strdup(Reference_Set[nr].Name[na]);
        initialize_coord_3D(&Model_Set[nr].x[na]);
    }
}

// Set up and initialize the output file
char    *outfile;
outfile=(char*)calloc(50,sizeof(char));
sprintf(outfile,"tmpFiles/out_rmsd.txt");
Oset.N=strdup(outfile);
Oset.F=myfopen(Oset.N,"w");
fprintf(Oset.F,"MODEL\tRMSD\t\n");

// Set up variables and structures for making comparsion between the reference and each model
double    sum=0.0, rmsd=0.0; // these do not depend on previous values
double    allsum=0.0; // this must be reset to 0.0 before each comparison
int       nr_model=0; // counter for residues in the comparison / model file
int       na_model=0; // counter for atoms in residues in the comparison / model file
residue  *this_model_res; // a residue pointer to make the code more readable
atom     *this_model_atom; // an atom pointer to make the code more readable
coord_3D  crdR,crdM; // coordinate holders to make the code more readable
// Loop over each molecule (model) in the comparison PDB file
for(i=0;i<Model_PDB[0].nm;i++){
    fprintf(Oset.F,"%d\t",i); // print the model's index in the assembly

    // Copy the coordinates from the Model_PDB into the Model_Set
    for ( nr=0 ; nr<nRings ; nr ++ ){
        for ( nr_model=0 ; nr_model<Model_PDB[0].m[i][0].nr ; nr_model++){
            this_model_res=&Model_PDB[0].m[i][0].r[nr_model];
            if ( this_model_res[0].n == Model_Set[nr].resNum ) {
                for ( na=0 ; na<Model_Set[nr].nA ; na++ ) {
		    for ( na_model=0 ; na_model<this_model_res[0].na ; na_model++ ) { 
	                this_model_atom=&this_model_res[0].a[na_model]; 
		        same = strcmp( Model_Set[nr].Name[na] , this_model_atom[0].N ); 
		        if ( same == 0 ) {
                            Model_Set[nr].x[na] = this_model_atom[0].x ;
			    break;
		        } // If the atoms names are the same, copy in the coords to the Model_Set
		    } // Loop over the corresponding atoms in the Model_PDB struct
                } // Loop over the atoms in the current ring in the Model_Set
            } // Does this model-pdb residue number == the current ring in the Model_Set
        } // Loop over residues in this model in the Model_PDB
    } // Loop over rings in the Model_Set.

// Uncomment the next block to test that the coordinates copied correctly
/** /
for ( nr=0 ; nr<nRings ; nr ++ ){
    printf("RESIDUE NUMBER: %d coordinates\n",Model_Set[nr].resNum);
    for ( na=0 ; na<Model_Set[nr].nA ; na++ ) {
	printf("ATOM %s : %4.2lf  %4.2lf  %4.2lf\n",
            Model_Set[nr].Name[na],
            Model_Set[nr].x[na].i,
            Model_Set[nr].x[na].j,
            Model_Set[nr].x[na].k);
    }
}
/ **/
      
    // Calculate the overall rmsd for each ring atom in the current model molecule
    for ( nr=0 ; nr<nRings ; nr ++ ){
    for ( na=0 ; na<Model_Set[nr].nA ; na++ ) { // could have also used Reference_Set[nr].nA
        crdR=Reference_Set[nr].x[na]; // this makes the 'sum' calculation line easier to read
	crdM=Model_Set[nr].x[na]; // this makes the 'sum' calculation line easier to read
        sum = pow(crdR.i-crdM.i,2) + pow(crdR.j-crdM.j,2) + pow(crdR.k-crdM.k,2); 
        allsum+=sum;
    }}
    
    // Take the root of the sum for this model / molecule and write the value to the output file
    //printf("%f\n",allsum);
    rmsd=sqrt(allsum/nTotalRingAtoms); 
    fprintf(Oset.F,"%lf\n",rmsd);
    //printf("%f\n",rmsd);
    allsum=0.0; // rmsd is over each model 

} // close loop over models (molecules)

fclose(Oset.F);

}

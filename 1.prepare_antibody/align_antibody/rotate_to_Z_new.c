/*
name:		rotate_to_Z.c
start date:	24th August 2010
usage:		rotate_to_Z <input-filename> <output-filestem>
*/

#include<glylib.h>


void rotate(vectormag_3D AD,char *assmb_filename,char *output_filename){
//printf("entered rotate\n");
int mi;
assembly *A;
//printf("assmb_filename is <<<%s>>>\n",assmb_filename);
//printf("output_filename is <<<%s>>>\n",output_filename);

A=load_pdb(assmb_filename);
	//printf("The number of molecules are %d\n",A[0].nm);
	for(mi=0;mi<A[0].nm;mi++)
	{
		//printf("\n********\nmolecule: %d\n\t#residues: %d\n",mi+1,A[0].m[mi][0].nr);
		rotate_vector_to_Z_M(A[0].m[mi],-1,-1,-1,-1,AD);
	}
outputAsmblPDB(A,output_filename);
}

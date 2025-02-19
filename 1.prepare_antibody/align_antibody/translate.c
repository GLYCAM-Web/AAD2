#include<glylib.h>

void translate(vectormag_3D A,char *assmb_filename,char *output_filename){
//printf("entered translate\n");

assembly *T;
T=load_pdb(assmb_filename);
int mi;
coord_3D co_A;
co_A=vec_to_coord(A);
for(mi=0;mi<T[0].nm;mi++){
	translate_molecule_by_XYZ(T[0].m[mi],-1,-1,co_A);
	}

outputAsmblPDB(T,output_filename);

}

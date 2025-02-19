#include <glylib.h>

vectormag_3D com_pdb(char *filename){
//printf("entered com_pdb\n");

atype *at = NULL;
int p=-1;
assembly* A;
int mi,ri,ai;
coord_3D com;
vectormag_3D vcom;

A=load_pdb(filename);
//printf("The number of molecules in this assembly are %d\n",A[0].nm);

for(mi=0;mi<A[0].nm;mi++){
for(ri=0;ri<A[0].m[mi][0].nr;ri++){
        for(ai=0;ai<A[0].m[mi][0].r[ri].na;ai++){
                if(strstr(A[0].m[mi][0].r[ri].a[ai].N,"C") != NULL){
                        A[0].m[mi][0].r[ri].a[ai].m=12.01;
                        //printf("The carbon atom name is %s\n",l.r[ri].a[ai].N);
                }
                if(strstr(A[0].m[mi][0].r[ri].a[ai].N,"H")!= NULL){
                        A[0].m[0][0].r[ri].a[ai].m=1.008;
                }
                if(strstr(A[0].m[mi][0].r[ri].a[ai].N,"N")!= NULL){
                        A[0].m[0][0].r[ri].a[ai].m=14.01;
                }
                if(strstr(A[0].m[mi][0].r[ri].a[ai].N,"O")!= NULL){
                       	A[0].m[0][0].r[ri].a[ai].m=16.00;
                }
                if(strstr(A[0].m[mi][0].r[ri].a[ai].N,"S")!= NULL){
                        A[0].m[0][0].r[ri].a[ai].m=32.07;
                }
        }
}
}
//printf("done\n");
com= get_assembly_molecule_COM(A,at,p);
//printf("the center of mass of CDRs are\t");
//dprint_coord_3D(&com);
vcom=coord_to_vec(com);


return vcom;
}

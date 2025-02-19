#include <glylib.h>


void extract_antibody(char *filename){
//printf("entered extract\n");

fileset Fset,Oset;
fileslurp Fslurp;
Oset.N=strdup("tmpFiles/antibody.pdb");
Oset.F=myfopen(Oset.N,"w");

Fset.N=strdup(filename);
Fslurp=slurp_file(Fset);
if(Fslurp.n==0){
	printf("no PDB file uploaded\n");
	exit(1);
}

int i=0;


for(i=0;i<Fslurp.n;i++){
	if(strstr(Fslurp.L[i],"ATOM") != NULL  || strstr(Fslurp.L[i],"TER") != NULL){	
		if(strstr(Fslurp.L[i],"HEADER") == NULL &&  strstr(Fslurp.L[i],"OBSLTE") == NULL &&  strstr(Fslurp.L[i],"TITLE") == NULL && strstr(Fslurp.L[i],"SPLIT") == NULL){
                if(strstr(Fslurp.L[i],"CAVEAT") == NULL && strstr(Fslurp.L[i],"COMPND") == NULL && strstr(Fslurp.L[i],"SOURCE") == NULL && strstr(Fslurp.L[i],"KEYWDS") == NULL ){
                        if(strstr(Fslurp.L[i],"EXPDTA") == NULL && strstr(Fslurp.L[i],"NUMMDL") == NULL && strstr(Fslurp.L[i],"MDLTYP") == NULL && strstr(Fslurp.L[i],"AUTHOR") == NULL){
                                if(strstr(Fslurp.L[i],"REVDAT") == NULL && strstr(Fslurp.L[i],"SPRSDE") == NULL && strstr(Fslurp.L[i],"JRNL") == NULL && strstr(Fslurp.L[i],"REMARK") == NULL){
                                        if(strstr(Fslurp.L[i],"DBREF") == NULL && strstr(Fslurp.L[i],"SEQADV") == NULL && strstr(Fslurp.L[i],"SEQRES") == NULL && strstr(Fslurp.L[i],"MODRES") == NULL){
                                                if(strstr(Fslurp.L[i],"HET") == NULL && strstr(Fslurp.L[i],"HETNAM") == NULL && strstr(Fslurp.L[i],"HETSYN") == NULL && strstr(Fslurp.L[i],"FORMUL") == NULL){
                                                        if(strstr(Fslurp.L[i],"HELIX") == NULL && strstr(Fslurp.L[i],"SHEET") == NULL && strstr(Fslurp.L[i],"SSBOND") == NULL && strstr(Fslurp.L[i],"LINK") == NULL){
                                                                if(strstr(Fslurp.L[i],"CISPEP") == NULL && strstr(Fslurp.L[i],"SITE") == NULL && strstr(Fslurp.L[i],"CRYST1") == NULL && strstr(Fslurp.L[i],"ORIG") == NULL){
                                                                        if(strstr(Fslurp.L[i],"SCALE") == NULL && strstr(Fslurp.L[i],"MTRIX") == NULL && strstr(Fslurp.L[i],"MODEL") == NULL && strstr(Fslurp.L[i],"ANISOU") == NULL && strstr(Fslurp.L[i],"HETATM") == NULL ){
                                                                                if(strstr(Fslurp.L[i],"CONECT") == NULL && strstr(Fslurp.L[i],"MASTER") == NULL){
                                        fprintf(Oset.F,"%s",Fslurp.L[i]);
                                                                                }
                                                                        }
                                                                }
                                                        }

                                                }
                                        }
                                }
                        }
                }

        }

        }

}
				
						
fclose(Oset.F);
Oset.F=0x0;
free(Oset.N);
free(Fset.N);
//printf("done\n");


}

/**breaks down the pdbqt output file from docking into individual models and reorders the residues into the right order***/
#include <glylib.h>

// This is the original declaration:
//int reorder(char *S,char *inputfile){
// We will use standard filenames from within the working directory, so no args needed.
// Also, we will not remove the files from the individual replicas
int main(){

fileset Fset,Mset,Pset;
fileslurp Fslurp,Pslurp;
assembly *m;
int i=0,j=1,k=0,l=0,num=0;

Fset.N=strdup("docking_output.pdbqt");
//printf("Fset.N is %s\n",Fset.N);
Fslurp = slurp_file(Fset);

char *filename=NULL;
filename=(char*)calloc(50,sizeof(char));

for(i=0;i<Fslurp.n;i++){
        if(strstr(Fslurp.L[i],"MODEL") != NULL){
                sprintf(filename,"tmpFiles/model_%d.pdb",j);
		Mset.N=strdup(filename);
		Mset.F=myfopen(Mset.N,"w");
		fprintf(Mset.F,"%s",Fslurp.L[i]);
        }
        if(strstr(Fslurp.L[i],"RESULT") != NULL){
		//printf("%s\n",Fslurp.L[i]);
		fprintf(Mset.F,"%s",Fslurp.L[i]);
	}


	if(strstr(Fslurp.L[i],"ATOM") != NULL || strstr(Fslurp.L[i],"HETATM") != NULL){
                //printf("%s\n",Fslurp.L[i]);
                fprintf(Mset.F,"%s",Fslurp.L[i]);

        }
        if(strstr(Fslurp.L[i],"ENDMDL") != NULL){
                fprintf(Mset.F,"%s",Fslurp.L[i]);
		j++;
		fclose(Mset.F);
        }
}

int e=0;


i=1;
int *resnos;
//printf(" j is %d\n",j);
char *reorder_pdb=NULL;
reorder_pdb=(char*)calloc(50,sizeof(char));
resnos=(int*)calloc(100,sizeof(int));
//sprintf(filename,"model_%d.pdb",i);
        //printf("%s\n",filename);
        //Pset.N=strdup(filename);
        //Pslurp=slurp_file(Pset);
	//m = load_pdb(filename);
	//printf("m[0].r[0].n is %d\n",m[0].m[0][0].r[0].na);
	//
molecule n;
n.nr=0;
n.r=(residue*)calloc(0,sizeof(residue));
for(i=0;i<j-1;i++){
	sprintf(filename,"tmpFiles/model_%d.pdb",i+1);
	//printf("%s\n",filename);
	Pset.N=strdup(filename);
	m = load_pdb(filename);
	sprintf(reorder_pdb,"tmpFiles/model%d_temp.pdb",i+1);
	//printf("%s\n",reorder_pdb);
	//printf("done\n");
	n.nr = m[0].m[0][0].nr;
	n.r=(residue*)realloc(n.r,(n.nr*sizeof(residue)));
	//printf("m.nr is %d\n",m[0].m[0][0].nr);
	//n[0].m[0][0].r=(residue*)calloc(n[0].m[0][0].nr,sizeof(residue));
	e=0;
	//resnos=(int*)calloc(m[0].m[0][0].nr,sizeof(int));
	
	for(k=0;k<m[0].m[0][0].nr;k++){
		//printf("k is %d and m.r[k].n is %d\n",k,m[0].m[0][0].r[k].n);
		resnos[k]=m[0].m[0][0].r[k].n;
	}


		
	for(k=0;k<m[0].m[0][0].nr;k++){
		for(l=0;l<m[0].m[0][0].nr-k-1;l++){
		if(resnos[l]>resnos[l+1]){
			e=resnos[l];
			resnos[l]=resnos[l+1];
			resnos[l+1]=e;
		}
		}
	}			

	for(k=0;k<m[0].m[0][0].nr;k++){
		//printf("resno is %d\n",resnos[k]);
	}



	for(k=0;k<m[0].m[0][0].nr;k++){
		for(l=0;l<m[0].m[0][0].nr;l++){
			if(resnos[k]==m[0].m[0][0].r[l].n){
				//printf("l is %d\n",l);
				n.r[k].n=m[0].m[0][0].r[l].n;
				n.r[k].na=m[0].m[0][0].r[l].na;
				n.r[k].a=(atom*)calloc(n.r[l].na,sizeof(atom));
				n.r[k]=m[0].m[0][0].r[l];	
			}
			//printf("%d\n",m.r[k].n);
		}
	}
	
	//printf("before outputMolPDB\n");
	num=n.nr;
	outputMolPDB(&n,reorder_pdb);

	free(Pset.N);
	n.nr=0;
		
	//sprintf(remove,"rm model_%d.pdb",i+1);
	//system(remove);

}


fileset Aset;
char *dfile;
dfile=(char*)calloc(50,sizeof(char));
sprintf(dfile,"docking_output.pdb");
fileset Dset;
Dset.N=strdup(dfile);
Dset.F=myfopen(Dset.N,"w");
char *afile;
afile=(char*)calloc(50,sizeof(char));
sprintf(afile,"allmodels.pdb");
Aset.N=strdup(afile);
Aset.F=myfopen(Aset.N,"w");

char *efile;
efile=(char*)calloc(50,sizeof(char));
char *mfile;
mfile=(char*)calloc(50,sizeof(char));
fileset Eset;
fileslurp Eslurp;
int mnum=0;
for(i=0;i<j-1;i++){
	sprintf(efile,"tmpFiles/model%d_temp.pdb",i+1);
	sprintf(mfile,"tmpFiles/model%d.pdb",i+1);
	sprintf(filename,"tmpFiles/model_%d.pdb",i+1);
	Pset.N=strdup(filename);
	Pset.F=myfopen(Pset.N,"r");
	Pslurp=slurp_file(Pset);
	Eset.N=strdup(efile);
	Eset.F=myfopen(Eset.N,"r");
	Mset.N=strdup(mfile);
	Mset.F=myfopen(Mset.N,"w");
	Eslurp=slurp_file(Eset);
	for(l=0;l<Pslurp.n;l++){
		if(strstr(Pslurp.L[l],"MODEL") != NULL){
			mnum++;
			fprintf(Aset.F,"MODEL %d\n",mnum);
			fprintf(Dset.F,"MODEL %d\n",mnum);
		}
		if(strstr(Pslurp.L[l],"RESULT") != NULL){
			fprintf(Aset.F,"%s",Pslurp.L[l]);
			fprintf(Mset.F,"%s",Pslurp.L[l]);
		}
	}
	for(k=0;k<Eslurp.n;k++){
		fprintf(Aset.F,"%s",Eslurp.L[k]);
		//make_ATOM_HETATM(Eslurp.L[k]);
		fprintf(Dset.F,"%s",Eslurp.L[k]);
		fprintf(Mset.F,"%s",Eslurp.L[k]);
		}
	fprintf(Aset.F,"TER\n");
	fprintf(Dset.F,"ENDMDL\n");
	fclose(Eset.F);
	free(Eset.N);
	fclose(Pset.F);
	free(Pset.N);
	fclose(Mset.F);
	free(Mset.N);
//	sprintf(remove,"rm model%d_temp.pdb model_%d.pdb",i+1,i+1);
//	system(remove);
}

fclose(Aset.F);
fclose(Dset.F);
return num;		
}

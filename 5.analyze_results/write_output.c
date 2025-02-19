#include <glylib.h>

// Write an output file that contains results from all the docking experiments

//In the original code, the declaration was this:
// int main(char *N,char *S) -- S is the project uuid.  N is the number of docking experiments
// Here, we won't need S, but we will still need N. But, we will call it ND.
int main(int argc,char *argv[]){

fileset Fset,Oset;
fileslurp Fslurp;
Oset.N=strdup("docking_output.pdbqt");
Oset.F=myfopen(Oset.N,"w");
int i=0,j=0;
int n=0,ND=0; // number of docking experiments: counter (n) and total (ND)

if(argc<2){
	printf("Write output requires one argument, an integer equal to the number of docking experiments.\n");
	printf("Exiting.\n");
	exit(1);
}
if(sscanf(argv[1],"%d",&ND)!=1){
	printf("Could not get an integer out of the argument. Exiting.\n");
	exit(1);
}

j=1;
for(n=0;n<ND;n++){
Fset.N=(char*)calloc(250,sizeof(char));
sprintf(Fset.N,"r%d/docking_output.pdbqt",(n+1));
Fslurp=slurp_file(Fset);
for(i=0;i<Fslurp.n;i++){
	if(strstr(Fslurp.L[i],"MODEL")!= NULL){
			fprintf(Oset.F,"MODEL %d\n",j);
			j++;
	}
	else{
			fprintf(Oset.F,"%s",Fslurp.L[i]);
		}
}
free(Fslurp.L);
free(Fset.N);
}

fclose(Oset.F);

return 0;
}

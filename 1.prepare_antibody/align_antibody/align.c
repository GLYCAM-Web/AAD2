#include <glylib.h>

void extract_antibody(char *filename);
void align_antibody(char *filename);

int main(int argc,char *argv[]){
//printf("entered align\n");

fileset Pset;

if(argc<2){
	printf("No PDB file uploaded\n");
	exit(1);
}

Pset.N=strdup(argv[1]);

extract_antibody(Pset.N);

align_antibody("tmpFiles/antibody.pdb");


return 0;
}

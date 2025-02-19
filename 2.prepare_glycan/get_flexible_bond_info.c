#include <glylib.h>

// Get the flexible 'bonds' to use in the pdbqt-generating script.
// It requires a rigid pdbqt file to be made first.
int main(int argc,char *argv[]){

fileset Sset;
fileslurp Lslurp;
int i,atom_num,inact_size=0;
char dum[1000];
int *inactive_atoms;
inactive_atoms=(int*)calloc(0,sizeof(int));
Sset.N=strdup("ligand.pdbqt");

char atom_name[10];
char atom_name_2[10];
char *atom_no;
char *atom_no_2;
Lslurp=slurp_file(Sset);
for(i=0;i<Lslurp.n;i++){
	//printf("%s\n",Lslurp.L[i]);
	if( strstr(Lslurp.L[i],"REMARK") != NULL){ 
		if( strstr(Lslurp.L[i],"C5") != NULL || strstr(Lslurp.L[i],"N") !=NULL){
		//REMARK    1  A    between atoms: O1_2  and  C1_3
		sscanf(Lslurp.L[i],"%s%s%s%s%s%s%s",dum,dum,dum,dum,atom_name,dum,atom_name_2);
		//printf("atom_name is %s\n",atom_name);
		//printf("atom_name_2 is %s\n",atom_name_2);
		atom_no=strtok(atom_name,"_");
		while(atom_no != NULL){
			//printf("atom_no is %s\n",atom_no);
			if(strstr(atom_no,"C") == NULL && strstr(atom_no,"N") == NULL){
			atom_num=atoi(atom_no);
			inact_size++;
			inactive_atoms=(int*)realloc(inactive_atoms,(inact_size*sizeof(int)));
			inactive_atoms[inact_size-1]=atom_num;
			//printf("number is %d\n",atom_num);
			}
			atom_no=strtok(NULL,"_");
		}

		atom_no_2=strtok(atom_name_2,"_");
                while(atom_no_2 != NULL){
                        //printf("atom_no is %s\n",atom_no_2);
                        if(strstr(atom_no_2,"C") == NULL && strstr(atom_no_2,"N") == NULL){
                        atom_num=atoi(atom_no_2);
                        inact_size++;
                        inactive_atoms=(int*)realloc(inactive_atoms,(inact_size*sizeof(int)));
                        inactive_atoms[inact_size-1]=atom_num;
                        //printf("number is %d\n",atom_num);
                        }
                        atom_no_2=strtok(NULL,"_");
		}
		}
	}
}
char *num_bonds = NULL;
char *bonds = NULL;
bonds=(char*)calloc(inact_size+inact_size-1,sizeof(char));
sprintf(bonds,"%d",inactive_atoms[0]-1);
num_bonds=(char*)calloc(inact_size+inact_size-1,sizeof(char));
for(i=1;i<inact_size;i++){
	sprintf(num_bonds,"_%d",inactive_atoms[i]-1);
	strcat(bonds,num_bonds);	
}

fprintf(stdout,"%s",bonds);

return 0;

}

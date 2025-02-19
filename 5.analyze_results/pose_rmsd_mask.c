#include <glylib.h>

void get_rmsd(char *reffile,char* allmodels,char *ringatomsfile){


/*if(argc<4 || argc>4){
	printf("USAGE ./get_rmsd reference.pdb allmodels.pdb ringatomsfile\n");
	}*/

fileset Cset,Pset,Oset;
assembly *r;
int i,j,k,l=0,m=0,n=0,test=0;

//printf("just inside get_rmsd\n");
Cset.N=strdup(reffile);
r=load_pdb(Cset.N);
//printf("just loaded %s The number of residues in this molecule are %d\n",reffile,r[0].m[0][0].nr);
coord_3D ref[r[0].m[0][0].nr*6];
int ref_size=0;

fileslurp Islurp;
fileset Iset;
Iset.N=strdup(ringatomsfile);
Islurp=slurp_file(Iset);
i=0;
j=0;
k=0;
char *atom;
atom=(char*)calloc(4,sizeof(char));
char **names;
names=(char**)calloc(r[0].m[0][0].nr*6,sizeof(char*));
int numrings=0;
int *ringsize;
ringsize=(int*)calloc(r[0].m[0][0].nr,sizeof(int));
coord_3D **ring;
ring=(coord_3D**)calloc(6,sizeof(coord_3D*));
while(i<Islurp.n){
        if(strstr(Islurp.L[i],"ATOM") != NULL){
                i++;
                while(strstr(Islurp.L[i],"END") == NULL){
                        //if(j>0) printf("1.  names[%d] is >>>%s<<<\n",j-1,names[j-1]);
                        sscanf(Islurp.L[i],"%s",atom);
                        names[j]=(char*)calloc(strlen(atom),sizeof(char));
                        names[j]=strdup(atom);
                        //printf("2.  names[%d] is >>>%s<<<\n",j,names[j]);
                        i++;
                        j++;
			ringsize[numrings]++;
                }
	        numrings++;
        }
        i++;
}


for(i=0;i<j;i++){
	//printf("names are %s\n",names[i]);
}

//printf("done with part 1\n");
i=0;
j=0;
k=0;
int check=0;
int ringnumber=0;
//printf("c.nr is %d\n",r[0].nm);
//printf("atoms are %d\n",r[0].m[0][0].nr);
for(i=0;i<r[0].m[0][0].nr;i++){
	if(i>0 && check==1){
		k=k+ringsize[ringnumber-1];
	}
	//printf("k is %d\n",k);
	n=k;
	//printf("n is %d\n",n);
	//printf("number of atoms are %d\n",r[0].m[0][0].r[i].na);	
	if(r[0].m[0][0].r[i].na >= 6){
	check=1;
	//printf("the size of ring number %d is %d\n",ringnumber, ringsize[ringnumber]);
	//for(test=0;test<6;test++)
	for(test=0;test<ringsize[ringnumber];test++){
	//printf("number of atoms are %d\n",r[0].m[0][0].r[i].na);
	for(j=0;j<r[0].m[0][0].r[i].na;j++){
		//printf("names[n=%d] is %s\n",n,names[n]);
		if(strcmp(r[0].m[0][0].r[i].a[j].N,names[n]) == 0){
		//printf("%s\n",r[0].m[0][0].r[i].a[j].N);
		//printf("ref_size is %d \n",ref_size);
		ref[ref_size]=r[0].m[0][0].r[i].a[j].x;
		ref_size++;		
		}
	}
	n++;	
	}//test
	ringnumber++;
	if(ringnumber>numrings){
		printf("This is ring number %d and the number of rings is %d\n",ringnumber, numrings);
		printf("something went wrong handling the rings in get_rmsd\n");
		exit(1);
	}
	}
	else{
		check=0;
	}
}

//printf("done with part 2\n");

for(i=0;i<ref_size;i++){
	//dprint_coord_3D(&ref[i]);
	}
//printf("done\n");
Pset.N=strdup(allmodels);
char *allfile;
allfile=(char*)calloc(50,sizeof(char));
sprintf(allfile,"%s",Pset.N);
//printf("<<<%s>>>\n",allfile);
assembly *A;
coord_3D mod[r[0].m[0][0].nr*6];
initialize_coord_3D(&mod[0]);
initialize_coord_3D(&mod[1]);
initialize_coord_3D(&mod[2]);
initialize_coord_3D(&mod[3]);
initialize_coord_3D(&mod[4]);
initialize_coord_3D(&mod[5]);
coord_3D diff[r[0].m[0][0].nr*6];
initialize_coord_3D(&diff[0]);
initialize_coord_3D(&diff[1]);
initialize_coord_3D(&diff[2]);
initialize_coord_3D(&diff[3]);
initialize_coord_3D(&diff[4]);
initialize_coord_3D(&diff[5]);

int mod_size=0;

int add;
coord_3D square;
initialize_coord_3D(&square);
A=load_pdb(allfile);
//printf("the number of molecules in this assembly are %d\n",A[0].nm);
double sum=0.0;
double allsum=0.0;
double rmsd=0.0;
int rescheck=0,resnum=0;
//printf("done\n");
m=0,test=0,n=0;
char *outfile;
outfile=(char*)calloc(50,sizeof(char));
sprintf(outfile,"tmpFiles/out_rmsd.txt");
Oset.N=strdup(outfile);
Oset.F=myfopen(Oset.N,"w");
fprintf(Oset.F,"MODEL\tRMSD\t\n");
for(i=0;i<A[0].nm;i++){
	fprintf(Oset.F,"%d\t",i);
	mod_size=0;
	resnum=0;
	m=0;
	n=0;
	check=0;
        ringnumber=0;
	for(j=0;j<A[0].m[i][0].nr;j++){
		//printf("residue number %d\n",j+1);
		if(j>0 && check ==1){
			m=m+ringsize[ringnumber-1];
		}
		n=m;
		add=0;
		rescheck=0;
		//printf("%d\n",A[0].m[i][0].r[j].na);
		if(A[0].m[i][0].r[j].na >=6){
		check=1;
		//printf("inside if and n is %d\n",n);
		for(test=0;test<ringsize[ringnumber];test++){
		for(k=0;k<A[0].m[i][0].r[j].na;k++){
			// printf("inside for and name is %s\n",names[n]);
			if(strcmp(A[0].m[i][0].r[j].a[k].N,names[n]) == 0){
			mod[mod_size]=A[0].m[i][0].r[j].a[k].x;
			mod_size++;
			rescheck++;		
			//printf("%s\n",A[0].m[i][0].r[j].a[k].N);	
			}
			
		}
		n++;
		}//test
		ringnumber++;
		if(ringnumber>numrings){
			printf("This is ring number %d and the number of rings is %d\n",ringnumber, numrings);
			printf("something went wrong handling the rings in get_rmsd\n");
			exit(1);
		}
		}
		else{
			check=0;
		}
		//printf("rescheck is %d\n",rescheck);
		if(rescheck == 6){
			//printf("inside if\n");
			resnum++;
		}

	}
	//printf("resnum is %d\n",resnum);
	 for(l=0;l<mod_size;l++){
			//dprint_coord_3D(&mod[l]);
			//dprint_coord_3D(&ref[l]);
                        diff[l] = subtract_coord(ref[l],mod[l]);
                        //dprint_coord_3D(&diff[l]);
                        square.i=diff[l].i*diff[l].i;
                        square.j=diff[l].j*diff[l].j;
                        square.k=diff[l].k*diff[l].k;
                        //dprint_coord_3D(&square);
			sum=square.i+square.j+square.k;
                        //printf("%lf\n",sum);
			allsum=allsum+sum;
                }
		//printf("%f\n",allsum);
		//printf("%d\n",resnum);
                rmsd=sqrt(allsum/(resnum*6));
               	fprintf(Oset.F,"%lf\n",rmsd);
		//printf("%f\n",rmsd);
		allsum=0.0;
		rmsd=0.0;
	


}

fclose(Oset.F);
//printf("Look for out_rmsd.txt file for results.\nThe output file is usually located in the same folder from which you run this program\n");


}

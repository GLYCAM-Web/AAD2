/*program for clustering, needs model.pdb allmodels.pdb and glycan ring atoms file)*/

#include<glylib.h>

void get_rmsd(char *reffile,char *allmodels,char *ringatomsfile);

int main(){


int i=0,j=0,k=0,l=0,m=0,n=0,r=0,modelno=0,model=0,check=0,modelsize=0;
double rmsd=0.0;
fileset Fset,Oset;
fileslurp Fslurp;
int *models;
models=(int*)calloc(modelsize,sizeof(int));
int mcheck=0,s=0,rcheck=0,cluster=0;
char *filename;
filename=(char*)calloc(50,sizeof(char));
sprintf(filename,"cluster.txt");
Oset.N=strdup(filename);
Oset.F=myfopen(Oset.N,"w");
char *rmsdfile;
rmsdfile=(char*)calloc(50,sizeof(char));
sprintf(rmsdfile,"tmpFiles/out_rmsd.txt");
char *allmodels;
allmodels=(char*)calloc(50,sizeof(char));
sprintf(allmodels,"allmodels.pdb");
char *ringatomsfile;
ringatomsfile=(char*)calloc(50,sizeof(char));
sprintf(ringatomsfile,"glycan_ring_atoms.txt");
char *ref;
ref=(char*)calloc(50,sizeof(char));
//printf("done\n");

j=1;
modelno=1;
while(i<j){
	k=0;
	l=1;
	r=modelsize;
	while(k<l){
		//printf("calculating rmsd for model %d\n",modelno);
		sprintf(ref,"tmpFiles/model%d.pdb",modelno);
		get_rmsd(ref,allmodels,ringatomsfile);
		//printf("done\n");
		k++;
	}
	//printf("done\n");
	//printf("%s\n",rmsdfile);
	Fset.N=strdup(rmsdfile);
	Fslurp=slurp_file(Fset);
	check=0;
	mcheck=0;
	rcheck=0;
//	printf("Fslurp.n is %d\n",Fslurp.n);
	for(m=0;m<Fslurp.n;m++){
		//printf("inside for\n");
		mcheck=0;
		rcheck=0;
		if(strstr(Fslurp.L[m],"RMSD") == NULL){
			sscanf(Fslurp.L[m],"%d%lf",&model,&rmsd);
			//printf("%d%lf",model,rmsd);
			if(rmsd<2.0){
				//printf("modelsize is %d\n",modelsize);
				for(s=0;s<modelsize;s++){
					if(models[s]== model+1){
						mcheck=1;
					}
				}
				if(mcheck==0){
					modelsize++;
					//printf("modelsize is %d\n",modelsize);
					models=(int*)realloc(models,(modelsize*sizeof(int)));
					//printf("done\n");
					//printf("modelsize-1 is %d\n",modelsize-1);
					models[modelsize-1]=model+1;

				}
			}
			if(rmsd>2.0){
				for(n=0;n<modelsize;n++){
					//printf("models[n] is %d and model+1 is %d\n",models[n],model+1);
					if(models[n]== model+1){
						//printf("models[n] is %d and model+1 is %d\n",models[n],model+1);
						rcheck=1;
					}
				}
				if(check==0 && rcheck==0){
					check=1;
					modelno=model+1;
					//printf("modelno is %d\n",model+1);
				}

			
			}
		}
	}	

fprintf(Oset.F,"Cluster %d\n",cluster+1);
//printf("Cluster %d\n",cluster+1);
while(r<modelsize){
	fprintf(Oset.F,"%d\n",models[r]);
	//printf("%d\n",models[r]);
	r++;
}

cluster++;

if(check==1){
i++;
j++;
}
else{
i++;
}

free(Fset.N);

}

fprintf(Oset.F,"END\n");
fclose(Oset.F);
//free(Fset.N);
free(Oset.N);
//printf("done with first loop\n");

fileset Aset;
fileslurp Aslurp;
char *Afile;
Afile=(char*)calloc(50,sizeof(char));
sprintf(Afile,"allmodels.pdb");
Aset.N=strdup(Afile);
Aslurp=slurp_file(Aset);
char *Cfile;
Cfile=(char*)calloc(50,sizeof(char));
sprintf(Cfile,"cluster.txt");
Fset.N=strdup(Cfile);
Fslurp=slurp_file(Fset);
char *Hfile;
Hfile=(char*)calloc(50,sizeof(char));
sprintf(Hfile,"cluster_histogram.txt");
Oset.N=strdup(Hfile);
Oset.F=myfopen(Oset.N,"w");

i=0;
int dum=0,modelnum=0,num_models=0,clusterno=0,csize=0;
double energy=0.0,total_energy=0.0,avg_energy=0.0;
char line[10];
int *cluster_nums;
cluster_nums=(int*)calloc(csize,sizeof(int));
int *total_models;
total_models=(int*)calloc(csize,sizeof(int));
double *avg_energies;
avg_energies=(double*)calloc(csize,sizeof(double));
check=0;
//printf("here\n");
while(i<Fslurp.n){
	check=0;
	total_energy=0.0;
	num_models=0;
	//printf("i is %d\n",i);
	//printf("inside for %s",Fslurp.L[i]);
	if(strstr(Fslurp.L[i],"Cluster")!=  NULL){
		i++;
		j=i;
		k=j+1;
		while(j<k){
			if(strstr(Fslurp.L[j],"Cluster")==NULL && strstr(Fslurp.L[j],"END") ==NULL){
				//printf("inside if %s\n",Fslurp.L[j]);
				sscanf(Fslurp.L[j],"%d",&dum);
				j++;
				k++;
			}
			else{
			 	//printf("inside else %s\n",Fslurp.L[j]);
			check=1;
			k=j;
			}
			if(check==0){
			for(l=0;l<Aslurp.n;l++){
				if(strstr(Aslurp.L[l],"MODEL")!=NULL){
					sscanf(Aslurp.L[l],"%s%d",line,&modelnum);
					if(modelnum==dum){
						l++;
						sscanf(Aslurp.L[l],"%s%s%s%lf",line,line,line,&energy);
						//printf("%d\t%lf\n",modelnum,energy);	
						total_energy=total_energy+energy;
						num_models++;
					}
				}
				
				
			}
			}
			}	
			
		
		}
					 	
	
	if(check==1){
		i=j;
		clusterno++;
		//printf("num_models are %d\n",num_models);
		avg_energy=total_energy/num_models;
		//printf("%lf\n",avg_energy);
		csize++;
		cluster_nums=(int*)realloc(cluster_nums,(csize*sizeof(int)));
		total_models=(int*)realloc(total_models,(csize*sizeof(int)));
		avg_energies=(double*)realloc(avg_energies,(csize*sizeof(double)));
		cluster_nums[csize-1]=clusterno;
		total_models[csize-1]=num_models;
		avg_energies[csize-1]=avg_energy;
		//fprintf(Oset.F,"%d\t%d\t%lf\n",clusterno,num_models,avg_energy);
		}
	else{
		i++;
	}

}
//fclose(Oset.F);

int temp_nummodels=0,temp_clusterno=0;
double temp_energy=0.0;
for(i=0;i<csize-1;i++){
	for(j=0;j<csize-1;j++){
		if(avg_energies[j+1]<avg_energies[j]){
			temp_energy=avg_energies[j];
			avg_energies[j]=avg_energies[j+1];
			avg_energies[j+1]=temp_energy;
			temp_nummodels=total_models[j];
			total_models[j]=total_models[j+1];
			total_models[j+1]=temp_nummodels;
			temp_clusterno=cluster_nums[j];
			cluster_nums[j]=cluster_nums[j+1];
			cluster_nums[j+1]=temp_clusterno;
		}
	}
}

fprintf(Oset.F,"# Cluster Number  Number of Poses  Average Interaction Energy\n");
for(i=0;i<csize;i++){
//	fprintf(Oset.F,"%d\t%d\t%lf\n",cluster_nums[i],total_models[i],avg_energies[i]);
	fprintf(Oset.F,"%d\t%d\t%.3lf\n",cluster_nums[i],total_models[i],avg_energies[i]);
}
fclose(Oset.F);
	
int ymodel=0;
for(i=0;i<csize;i++){
	if(total_models[i]>ymodel){
		ymodel=total_models[i];
	}
}

return 0;
}

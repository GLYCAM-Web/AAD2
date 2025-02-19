#include <glylib.h>
#include <gly_fileutils.h> 

/*** Notes: load_pdb_from_slurp :If there are any  insertions it is stored in the same residue number and also it reads the file only until a TER card**/
int  CDR_regions(fileslurp Islurp,char *lfile,char *hfile,char *sfile,char *cfile){
//printf("entered CDR\n");

fileset Aset,Pset,Hset,Lset,Sset;
int i=0,j=1,ri=0,tercheck=0,l1check=0,l2check=0,l3check=0,echeck=0,h1check=0,h2check=0,h3check=0,h1res=0,resnum_check=0;
int ccheck=0;
assembly *l,*h;
char *ab_chain;
sprintf(sfile,"tmpFiles/scfv.pdb");
sprintf(lfile,"tmpFiles/light.pdb");
sprintf(hfile,"tmpFiles/heavy.pdb");
Lset.N = strdup(lfile);
//printf("<<<%s>>>>\n",Lset.N);
Lset.F = myfopen(Lset.N,"w");
Hset.N = strdup(hfile);
//printf("<<<%s>>>>\n",Hset.N);
Hset.F = myfopen(Hset.N,"w");
Sset.N = strdup(sfile);
Sset.F = myfopen(Sset.N,"w");
//printf("done setting file names\n");

ab_chain=strdup("tmpFiles/antibody_onechain.pdb");
Aset.N=strdup(ab_chain);
Aset.F=myfopen(Aset.N,"w");
//printf("done setting file name\n");


int chain_check=0;
for(i=0;i<Islurp.n;i++){

if(chain_check<2){
	fprintf(Aset.F,"%s",Islurp.L[i]);
}

if(strstr(Islurp.L[i],"TER") != NULL){
	chain_check++;
	}

}

fclose(Aset.F);
//printf("done writing antibody_onechain\n");
fileslurp Pslurp;
Pset.N=strdup(ab_chain);
Pslurp=slurp_file(Pset);


//extracting the residue numbers for framework regions

if(Pslurp.n==0){
	printf("ATOM cards and coordinates not present in the PDB file\n");
	//exit(1);
}
		
char dum[1000];
int dum_no,ter_no;
int *frame_no= NULL;
frame_no=(int*)calloc(2,sizeof(int));
//printf("The number od lines are %d\n",Pslurp.n);
for(i=0;i<Pslurp.n;i++){
	if(tercheck==0){
		if(strstr(Pslurp.L[i],"TER") != NULL && i+1<Pslurp.n){
			//printf("%d\n",i+1);
			sscanf(Pslurp.L[i+1],"%s%d%s%s%s%d",dum,&dum_no,dum,dum,dum,&ter_no);
			//printf("%d\n",ter_no);
			frame_no[0]=ter_no;
			tercheck=1;
		}
	}
}
//printf("done with for loop\n");
frame_no[1]=frame_no[0]+112;
int lcheck=0,hcheck=0;
//printf("frame_no[0] and frame_no[1] are %d and  %d\n",frame_no[0],frame_no[1]);
if(tercheck ==1){
tercheck=0;
/** Slurping in the file and writing thelight and heavy framework pdb files seperately..should rewrite this part by using the load_pdb*/
for(i=0;i<Pslurp.n;i++){
	sscanf(Pslurp.L[i],"%s%d%s%s%s%d",dum,&dum_no,dum,dum,dum,&ter_no);
	//printf("ter_no is %d\n",ter_no);
	if(strstr(Pslurp.L[i],"ATOM") != NULL){
		//printf("%s",Islurp.L[i]);
		//printf("ter_no is %d\n",ter_no);
		fprintf(Sset.F,"%s",Pslurp.L[i]);
		if(tercheck==0 && ter_no <= 110){
		lcheck=1;
		//printf("%s",Islurp.L[i]);
		fprintf(Lset.F,"%s",Pslurp.L[i]);
		}
		if(tercheck==1 && ter_no <= frame_no[1]){
		hcheck=1;
		//printf("%s",Islurp.L[i]);
		fprintf(Hset.F,"%s",Pslurp.L[i]);
		}
	}
	if(strstr(Pslurp.L[i],"TER") != NULL){
		fprintf(Sset.F,"%s",Pslurp.L[i]);
		if(tercheck==0){
		}
		tercheck =1;		
	}
}

fclose(Hset.F);
fclose(Lset.F);
fclose(Sset.F);
}
else{
//printf("Heavy chain not found\n");
//exit(1);
}

//printf("done writing light and heavy chains\n");
/** The array loop will store the residue numbers of all the residues in both the light and heavy chain CDR regions. The array l_resnum is used to store resnumbers while going through the PDB file to extract the CDR regions***/
i=0;
int *loop;
loop=(int*)calloc(0,sizeof(int));
int loopsize=0;
int *l_resnum;
l_resnum=(int*)calloc(0,sizeof(int));
int rsize=0;

l = load_pdb(lfile);
//printf("The number of molecules in this assembly are %d\n",l[0].nm);
//printf("The number of residues are %d\n",l[0].m[0][0].nr);
ri=0;
if(lcheck==1){
while(ri<l[0].m[0][0].nr){
	
	if ( l1check == 0 && l[0].m[0][0].r[ri].n == 21 && strstr(l[0].m[0][0].r[ri].N,"CYS")!= NULL){
		l1check=1;
		loopsize++;
		loop= (int*)realloc(loop,(loopsize*sizeof(int)));
		loop[loopsize-1]= l[0].m[0][0].r[ri+1].n;	
		
		//printf("l1 starts at %d\n",l[0].m[0][0].r[ri+1].n);
		rsize++;
		l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
		l_resnum[rsize-1]=loop[loopsize-1]+11;
		}
	else{
		if(l1check==0 && l[0].m[0][0].r[ri].n > 21 && strstr(l[0].m[0][0].r[ri].N,"CYS") != NULL){
			l1check=1;
			loopsize++;
                	loop= (int*)realloc(loop,(loopsize*sizeof(int)));
                	loop[loopsize-1]= l[0].m[0][0].r[ri+1].n;
			//printf("l1 starts at %d\n",l[0].m[0][0].r[ri+1].n);
			rsize++;
	                l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
        	        l_resnum[rsize-1]=loop[loopsize-1]+10;
			//printf("l_resnum is %d\n",l_resnum[rsize-1]);
		}
	}	
	if (l1check ==1 && l2check ==0){
		if(l[0].m[0][0].r[ri].n >= l_resnum[rsize-1]){
			if(strstr(l[0].m[0][0].r[ri].N,"TRP") != NULL){
				loopsize++;
				loop=(int*)realloc(loop,(loopsize*sizeof(int)));
				loop[loopsize-1]=l[0].m[0][0].r[ri-1].n;
				//printf("l1 ends at %d\n",l[0].m[0][0].r[ri-1].n);
				l2check=1;
				loopsize++;
				loop=(int*)realloc(loop,(loopsize*sizeof(int)));
				loop[loopsize-1]=l[0].m[0][0].r[ri-1].n +16;
				//printf("l2 starts at %d\n",l[0].m[0][0].r[ri-1].n +16);
				loopsize++;
				loop=(int*)realloc(loop,(loopsize*sizeof(int)));
				loop[loopsize-1]=loop[loopsize-2]+6;
				//printf("l2 ends at %d\n",loop[loopsize-2]+6);
				if(loop[0]>24){
					rsize++;
					l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
                                	l_resnum[rsize-1]=loop[loopsize-1]+22;
				}
				else{
					rsize++;
					l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
					l_resnum[rsize-1]=loop[loopsize-1]+32;
				}
				l2check =1;
			}
		}
	}
	
	if(l2check ==1 && l3check == 0){
		if(l[0].m[0][0].r[ri].n >= l_resnum[rsize-1]){
			if(strstr(l[0].m[0][0].r[ri].N,"CYS") != NULL){
				loopsize++;
				loop=(int*)realloc(loop,(loopsize*sizeof(int)));
				loop[loopsize-1]=l[0].m[0][0].r[ri+1].n;
				//printf("l3 starts at %d\n",l[0].m[0][0].r[ri+1].n);
				rsize++;
				l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
				l_resnum[rsize-1]= loop[loopsize-1]+7;
				//printf("l_resnum is %d\n",l_resnum[rsize-1]);
				l3check=1;
			}
		}
	}
	
	if(l3check == 1 && echeck ==0){
		if(l[0].m[0][0].r[ri].n >= l_resnum[rsize-1]){
			if(strstr(l[0].m[0][0].r[ri].N,"PHE") != NULL){
				if( ri+1<l[0].m[0][0].nr && strstr(l[0].m[0][0].r[ri+1].N,"GLY") != NULL){
					loopsize++;
					loop=(int*)realloc(loop,(loopsize*sizeof(int)));
					loop[loopsize-1]=l[0].m[0][0].r[ri-1].n;
					echeck =1;
				}
				else{
					if(ri+1== l[0].m[0][0].nr && echeck==0){
					loopsize++;
                                        loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                        loop[loopsize-1]=l[0].m[0][0].r[ri-1].n;
                                        echeck =1;
					}
				}
			}
			else{
				if(ri+1== l[0].m[0][0].nr && echeck==0){
					loopsize++;
                                        loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                        loop[loopsize-1]=l[0].m[0][0].r[ri-1].n;
                                        echeck =1;
				}
			}
		}
	}
	
	
ri++;
}
}//iflcheck
//printf("done with light chain frame work\n");
//printf("loop size is %d\n",loopsize);

if(loopsize<6){
	printf("Not able to identify variable chains in the light chain\n");
	//exit(1);
}

for(i=0;i<loopsize;i++){
	//printf("light chain residues are %d\n",loop[i]);
}
echeck=0;
ri=0;
if(hcheck==1){
h = load_pdb(hfile);
//printf("the number of molecules in heavy pdb are %d\n",h[0].nm);
//printf("first resnum i heavyy chain is %s\n",h[0].m[0][0].r[0].N);
//printf("The number of residues are %d\n",h[0].m[0][0].nr);

ri=0;
//adding this for testing alignment just the if statement
if(loopsize==6){
while(ri<h[0].m[0][0].nr){
	//printf("%d\n",h[0].m[0][0].r[ri].n);	
	if(resnum_check ==0){
		if(h[0].m[0][0].r[ri].n ==1){
			h1res=21;
			//printf("h1res is %d\n",h1res);
			resnum_check=1;
		}
		else{
			if(h[0].m[0][0].r[ri].n >1  && h[0].m[0][0].r[ri].n>100){
				//printf("the first res number in heavy chain is %d\n",h[0].m[0][0].r[ri].n);
				h1res = h[0].m[0][0].r[ri].n + 20;
				//printf("here h1res is %d\n",h1res);
				resnum_check=1;
			}
			else{
				h1res=21;
			}
		}	
	}
	//printf("The residue number is %d and the resname is %s\n",h[0].m[0][0].r[ri].n,h[0].m[0][0].r[ri].N);
        if ( h1check == 0 && h[0].m[0][0].r[ri].n == h1res && strstr(h[0].m[0][0].r[ri].N,"CYS")!= NULL){
                //printf("h1 res is %d and inside h1\n",h1res);
		//printf("The resnumber is %d\n",h[0].m[0][0].r[ri+4].n);
		h1check=1;
                loopsize++;
                loop= (int*)realloc(loop,(loopsize*sizeof(int)));
                loop[loopsize-1]= h[0].m[0][0].r[ri+4].n;
                rsize++;
                l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
                l_resnum[rsize-1]=loop[loopsize-1]+8;
		//printf("%d loop\n",loop[loopsize-1]+8);
                }
        else{
                if(h1check==0 && h[0].m[0][0].r[ri].n > h1res && strstr(h[0].m[0][0].r[ri].N,"CYS") != NULL){
                        //printf("inside h1");
                	//printf("h1 residue is %d\n",h[0].m[0][0].r[ri].n);
			h1check=1;
                        loopsize++;
                        loop= (int*)realloc(loop,(loopsize*sizeof(int)));
                        loop[loopsize-1]= h[0].m[0][0].r[ri+4].n;
                        rsize++;
                        l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
                        l_resnum[rsize-1]=loop[loopsize-1]+8;

                }
        }
        if (h1check ==1 && h2check ==0){
                //printf("h1 starts at %d\n",loop[loopsize-1]);
		if(h[0].m[0][0].r[ri].n >= l_resnum[rsize-1]){
                        if(strstr(h[0].m[0][0].r[ri].N,"TRP") != NULL){
                                loopsize++;
                                loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                loop[loopsize-1]=h[0].m[0][0].r[ri-1].n;
				//printf("h1 ends at %d\n",loop[loopsize-1]);
                                h2check=1;
                                loopsize++;
                                loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                loop[loopsize-1]=h[0].m[0][0].r[ri-1].n +15;
				//printf("h2 starts at %d\n",loop[loopsize-1]);
                                loopsize++;
                                loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                loop[loopsize-1]=loop[loopsize-2]+9;
				//printf("h2 ends at %d\n",loop[loopsize-2]+9);
                              	//printf("looop[7] is %d and h1res is %d\n",loop[7],h1res+5);
				if(loop[6]>h1res+5){
				//printf("inside if\n");
				rsize++;
				l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
                                l_resnum[rsize-1]=loop[loopsize-1]+20;
				}
				else{
				rsize++;
                                l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
                                l_resnum[rsize-1]=loop[loopsize-1]+26;
				}
				
                                //printf(" l_resnum is %d\n",l_resnum[rsize-1]);
				h2check =1;
                        }
                }
        }

        if(h2check ==1 && h3check == 0){
                if(h[0].m[0][0].r[ri].n >= l_resnum[rsize-1]){
                        if(strstr(h[0].m[0][0].r[ri].N,"CYS") != NULL){
                                loopsize++;
                                loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                loop[loopsize-1]=h[0].m[0][0].r[ri+3].n;
				//printf(" h3 residue starts at  %d\n",h[0].m[0][0].r[ri+3].n);
                                rsize++;
                                l_resnum=(int*)realloc(l_resnum,(rsize*sizeof(int)));
                                l_resnum[rsize-1]= loop[loopsize-1]+8;
                                //printf("l_resnum is %d\n",l_resnum[rsize-1]);
				h3check=1;
                        }
                }
                       

      }

        if(h3check == 1 && echeck ==0){
		//printf("inside h3\n");
		//printf("%d and %d\n",h[0].m[0][0].r[ri].n,l_resnum[rsize-1]);
                if(h[0].m[0][0].r[ri].n >= l_resnum[rsize-1]){
                        //printf("inside if \n");
			if(strstr(h[0].m[0][0].r[ri].N,"TRP") != NULL){
                                //printf("inside second if \n");
				//printf("%s\n",h[0].m[0][0].r[ri].N);
				//printf("%s\n",h[0].m[0][0].r[ri+1].N);
				//printf("%d\n",ri+1);
				if(ri+1<h[0].m[0][0].nr && strstr(h[0].m[0][0].r[ri+1].N,"GLY") != NULL){
                                        //printf("inside third if\n");
					loopsize++;
                                        loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                        loop[loopsize-1]=h[0].m[0][0].r[ri-1].n;
                                        //printf("h3 ends at %d\n",h[0].m[0][0].r[ri-1].n);
					echeck =1;
                                }
				else{
					if(ri+1==h[0].m[0][0].nr && echeck==0){
						loopsize++;
                                        	loop=(int*)realloc(loop,(loopsize*sizeof(int)));
                                        	loop[loopsize-1]=h[0].m[0][0].r[ri-1].n;
						//printf("h3 ends at %d\n",h[0].m[0][0].r[ri-1].n);
						echeck=1;
					}
				}//else
                        }
			else{
				//printf("resnum is %d and residue is %s\n",h[0].m[0][0].r[ri].n,h[0].m[0][0].r[ri].N);
				//printf("echeck is %d\n",echeck);	
				//printf("ri+1 is %d\n",ri+1);
				//printf("nr is %d\n",h[0].m[0][0].nr);
				if(ri+1==h[0].m[0][0].nr && echeck==0){
					loopsize++;
                                        loop=(int*)realloc(loop,(loopsize*sizeof(int)));
					//printf("l_resnum is %d\n",l_resnum[rsize-1]);
                                        loop[loopsize-1]=l_resnum[rsize-1];
                                        //printf("h3 ends at %d\n",h[0].m[0][0].r[ri-1].n);
				}
			}
                }
        }

//printf("outside\n");
ri++;
}
}//ifloopsize
}//if hcheck
//printf("done with heavy chain framework\n");
//printf("loopsize is %d\n",loopsize);
if(loopsize<12){
	printf("Not able to identify variable chains in the heavy chain\n");
	//exit(1);
}

//printf("loop size is %d\n",loopsize);
for(i=0;i<loopsize;i++){
        //printf("the resnums of loops are %d\n",loop[i]);
        }

i=0;
int k =0,lc_num=0;
molecule lc;
lc.nr=0;
lc.r=(residue*)calloc(0,sizeof(residue));
//printf("loopsize is %d\n",loopsize);
if(loopsize==12){
while(i<loopsize){
        //printf("inside while\n");
	j=loop[i];
	//printf("j is %d\n",j);
        k=0;
        if(i<=4){
	//printf("inside if\n");
	//printf("k is %d and L[0].m[0][0].nr is %d\n",k,l[0].m[0][0].nr);
	while(k<l[0].m[0][0].nr ){
                //printf("k is %d and resname is %s\n",k,l.r[k].N);
                //printf("k is %d and resnum in l is %d\n",j,l[0].m[0][0].r[k].n);
		if(l[0].m[0][0].r[k].n== j){
                //printf("j is %d and resnum in l is %d\n",j,l[0].m[0][0].r[k].n);     
                 while(l[0].m[0][0].r[k].n <= loop[i+1]){
                        //printf("k is %d and resname is %s\n",k,l.r[k].N);
                        lc.nr++;
                        lc_num=lc.nr-1;
                        lc.r=(residue*)realloc(lc.r,(lc.nr*sizeof(residue)));
                        lc.r[lc.nr-1]= l[0].m[0][0].r[k];
                        //printf("lc resname is %s\n",lc.r[lc.nr-1].N);
                        //printf("%d\n",k);
                        k++;
                }

                }
        k++;
        }
	}
	if(i>4){
		 while(k<h[0].m[0][0].nr ){
                //printf("k is %d and resname is %s\n",k,l.r[k].N);
                //printf("j is %d and resnum in l is %d\n",j,l[0].m[0][0].r[k].n);
                if(h[0].m[0][0].r[k].n== j){
                //printf("j is %d and resnum in l is %d\n",j,l[0].m[0][0].r[k].n);     
                 while(h[0].m[0][0].r[k].n <= loop[i+1]){
                        //printf("k is %d and resname is %s\n",k,l.r[k].N);
                        lc.nr++;
                        lc_num=lc.nr-1;
                        lc.r=(residue*)realloc(lc.r,(lc.nr*sizeof(residue)));
                        lc.r[lc.nr-1]= h[0].m[0][0].r[k];
                        //printf("lc resname is %s\n",lc.r[lc.nr-1].N);
                        //printf("%d\n",k);
                        k++;
                }

                }
        k++;
        }
        }


i=i+2;
//printf("i is %d\n",i);

}

}

for(i=0;i<lc.nr;i++){
        //printf("the resname is %s\n",lc.r[i].N);
        for(ri=0;ri<lc.r[i].na;ri++){
                //printf("the atomname is %s\n",lc.r[i].a[ri].N);
        }
}



/*char *CDR = NULL;
CDR=(char*)calloc(8,sizeof(char));
sprintf(CDR,"CDR.pdb");*/
if(lc.nr>1){
ccheck=1;
outputMolPDB(&lc,cfile);
}

//printf("here\n");
Hset.F=0x0;
free(Hset.N);
Lset.F=0x0;
free(Lset.N);
//printf("Hset.N is %s\n",Hset.N);
return ccheck;
free(lfile);
free(hfile);

}

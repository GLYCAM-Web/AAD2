#include <glylib.h>

int CDR_regions(fileslurp Islurp,char *lfile,char *hfile,char *sfile,char *cfile);
vectormag_3D com_pdb(char *filename);
vectormag_3D D(vectormag_3D A,vectormag_3D B,vectormag_3D C);
void rotate(vectormag_3D AD,char *assmb_filename,char *output_filename);
void translate(vectormag_3D,char *assmb_filename,char *output_filename);
double angle(coord_3D B,coord_3D C);

void align_antibody(char *filename){
//printf("entered align_anitbody\n");

fileset Pset;
fileslurp Pslurp;
vectormag_3D light_com,heavy_com,cdr_com,AD_rotate;
char *lfile, *hfile, *sfile, *cfile;
int testing=0;

lfile=strdup("tmpFiles/light.pdb");
hfile=strdup("tmpFiles/heavy.pdb");
sfile=strdup("tmpFiles/scfv.pdb");
cfile=strdup("tmpFiles/cdr.pdb");
char *l_rotate=NULL,*s_rotate=NULL,*h_rotate=NULL,*c_rotate=NULL;
char *l_translate=NULL,*h_translate=NULL,*c_translate=NULL,*s_translate=NULL;

Pset.N=strdup("tmpFiles/antibody.pdb");
Pslurp=slurp_file(Pset);
if(Pslurp.n==0){
	printf("ATOM cards and coordinates not found\n");
	exit(1);
}
//printf("identifying CDR regions\n");
testing=CDR_regions(Pslurp,lfile,hfile,sfile,cfile);
//printf("done identifying\n");

// center of mass of the CDR regions (A).light chain framework region(B),heavychain framework region(C);
if(testing==1){
light_com = com_pdb(lfile);
heavy_com = com_pdb(hfile);
cdr_com = com_pdb(cfile);

//printf("before rotate\n");

//getting the rotate values
AD_rotate=D(cdr_com,light_com,heavy_com);
l_rotate=strdup("tmpFiles/light_rotate.pdb");
rotate(AD_rotate,lfile,l_rotate);
h_rotate=strdup("tmpFiles/heavy_rotate.pdb");
rotate(AD_rotate,hfile,h_rotate);
c_rotate=strdup("tmpFiles/cdr_rotate.pdb");
rotate(AD_rotate,cfile,c_rotate);
s_rotate=strdup("tmpFiles/scfv_rotate.pdb");
rotate(AD_rotate,sfile,s_rotate);


//center of mass of the CDR regions (A).light chain framework region(B),heavychain framework region(C) after rotating;
light_com = com_pdb(l_rotate);
heavy_com = com_pdb(h_rotate);
cdr_com = com_pdb(c_rotate);


int sign = -1;
cdr_com.i=cdr_com.i*sign;
cdr_com.j=cdr_com.j*sign;
cdr_com.k=cdr_com.k*sign;


l_translate=strdup("tmpFiles/light_translate");
translate(cdr_com,l_rotate,l_translate);
h_translate=strdup("tmpFiles/heavy_translate");
translate(cdr_com,h_rotate,h_translate);
c_translate=strdup("tmpFiles/cdr_translate");
translate(cdr_com,c_rotate,c_translate);
s_translate=strdup("tmpFiles/scfv_translate");
translate(cdr_com,s_rotate,s_translate);
//printf("after translate\n");


light_com = com_pdb(l_translate);
heavy_com = com_pdb(h_translate);
cdr_com = com_pdb(c_translate);

//calculating the angle for yawing
double angle_yaw;
coord_3D B,C;
B=vec_to_coord(light_com);
C=vec_to_coord(heavy_com);
angle_yaw = angle(B,C);

assembly *T;
T=load_pdb(s_translate);
int i;
for(i=0;i<T[0].nm;i++){
	yawMolecule(T[0].m[i],angle_yaw);
	}

char *s_yawed;
s_yawed=strdup("scfv_aligned.pdb");
outputAsmblPDB(T,s_yawed);
//printf("done here\n");

}

}

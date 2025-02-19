#include<glylib.h>

vectormag_3D D(vectormag_3D A,vectormag_3D B,vectormag_3D C){
//printf("entered D\n");


vectormag_3D vec_D,AB,BC,BD,AD;
double X;
AB=subtract_vec(B,A);
BC=subtract_vec(C,B);
X=(AB.d*AB.d)/(BC.d*BC.d);

BD.i=BC.i*X;
BD.j=BC.j*X;
BD.k=BC.k*X;

vec_D=add_vec(BD,B);

AD=subtract_vec(A,vec_D);


return AD;

}

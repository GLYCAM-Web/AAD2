GLYLIB = /programs/src/AAD2_GLYLIB/lib
GLYINC = /programs/src/AAD2_GLYLIB/inc
FLAGS = -I$(GLYINC) -L$(GLYLIB) -lglylib -Wall -lm
SOURCE = align.c
FUNCTION = align_antibody.c
FUNCTION1 = CDR_framework.c
FUNCTION2 = com_molecule.c
FUNCTION3 = get_D.c
FUNCTION4 = rotate_to_Z_new.c
FUNCTION5 = translate.c
FUNCTION6 = get_angle.c
FUNCTION8 = extract_framework.c
OUTEXE = align


default:
	 gcc $(SOURCE) $(FUNCTION) $(FUNCTION1) $(FUNCTION2) $(FUNCTION3) $(FUNCTION4) $(FUNCTION5) $(FUNCTION6) $(FUNCTION7) $(FUNCTION8) $(FUNCTION9) $(FLAGS)  -g -o $(OUTEXE)

gdb:
	gcc $(SOURCE) $(FLAGS) -g -o $(OUTEXE)

eventually:
	gcc test_final.c outputpdb_fromslurp.c $(FLAGS) -g -o $(OUTEXE)

#test:
#       gcc $(TESTIN) $(FLAGS) -g -o testexe

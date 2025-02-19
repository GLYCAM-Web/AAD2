GLYLIB = /programs/src/AAD2_GLYLIB/lib
GLYINC = /programs/src/AAD2_GLYLIB/inc
FLAGS = -I$(GLYINC) -L$(GLYLIB) -lglylib -Wall -lm -fcommon
SOURCE1 = write_output.c
OUTEXE1 = write_output
SOURCE2 = reorderpdb.c
OUTEXE2 = reorderpdb
#SOURCE3 = extract_ringatoms.c
#OUTEXE3 = extract_ringatoms
SOURCE4 = clustering.c
FUNCTION4 = pose_rmsd_mask.c
OUTEXE4 = clustering

all: writeout reorder cluster

writeout:
	 gcc $(SOURCE1) $(FLAGS) -g -o $(OUTEXE1)

reorder:
	 gcc $(SOURCE2) $(FLAGS) -g -o $(OUTEXE2)

#extract:
#	 gcc $(SOURCE3) $(FLAGS) -g -o $(OUTEXE3)

cluster:
	 gcc $(SOURCE4) $(FUNCTION4) $(FLAGS) -g -o $(OUTEXE4)


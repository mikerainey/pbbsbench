include common/parallelDefs

COMMON = common/IO.h pbbslib/parse_command_line.h common/geometry.h common/geometryIO.h pbbslib/parallel.h pbbslib/sequence.h
GENERATORS = randPoints
TO_UPDATE = triangles addRays toNodes

.PHONY: all clean
all: $(GENERATORS)
install:
	cp $(GENERATORS) $(INSTALL_FOLDER)

%.o : %.C $(COMMON) 
	$(CC) $(CFLAGS) -c $< -o $@

randPoints : randPoints.C geometryData.h $(COMMON) 
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $@.C

toNodes : toNodes.C $(COMMON) 
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $@.C

triangles : triangles.C geometryData.h $(COMMON) 
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $@.C

addRays : addRays.C $(COMMON) 
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $@.C

clean :
	rm -f *.o $(GENERATORS)
	make clean -s -C data

cleansrc : 
	make -s clean
	rm -f $(COMMON) 

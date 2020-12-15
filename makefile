CC = gcc
LIB = -lCovid 
DIR = $(shell pwd)

ODIR ?= Covid19Assignments
BINS := $(subst .c,.out,$(shell find . -name "main*.c"))
LIBS := $(subst .c,.so,$(shell find . -name "lib*.c"))


covid19: $(LIBS) $(BINS)
#	mkdir -p $(ODIR)
	export LD_LIBRARY_PATH=$(DIR)

%.o: %.c
#	$(info compiling c file)
	$(CC) -c $^ -o $@ 

# make the shared librarys files
lib%.so: lib%.c
#	$(info Compiling shared lib)
	$(CC) $^ -shared -fPIC -o $@

%.out: $(LIBS) %.o
#	$(info compiling out file)
	$(CC) -g3 $(subst .out,.o,$@) $(LIB) -Wl,-rpath,$(DIR),--enable-new-dtags -L$(DIR) -o $@

clean:
	rm *.out *.so

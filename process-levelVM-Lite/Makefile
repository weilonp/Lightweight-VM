#-----------------------------------------------------------------------------
# Makefile
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Choose a compiler and its options
#--------------------------------------------------------------------------
CC   = gcc -std=gnu99	# Use gcc for Zeus
OPTS = -Og -Wall -Werror -Wno-error=unused-variable -Wno-error=unused-function -pthread
DEBUG = -g					# -g for GDB debugging

#--------------------------------------------------------------------
# Build Environment
#--------------------------------------------------------------------
SRCDIR=./src
OBJDIR=./obj
INCDIR=./inc
BINDIR=.

#--------------------------------------------------------------------
# Build Files
#--------------------------------------------------------------------
SRCS=$(wildcard $(SRCDIR)/vm*.c)

#--------------------------------------------------------------------
# Compiler Options
#--------------------------------------------------------------------
INCLUDE=$(addprefix -I,$(INCDIR))
LIBRARY=$(addprefix -L,$(OBJDIR))
SRCOBJS=${SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o}
OBJS=$(OBJDIR)/vm.o $(OBJDIR)/vm_cs.o $(OBJDIR)/vm_shell.o $(OBJDIR)/vm_schedule.o $(OBJDIR)/vm_support.o
CFLAGS=$(OPTS) $(INCLUDE) $(LIBRARY) $(DEBUG)

#--------------------------------------------------------------------
# Build Recipies for the Executables (binary)
#--------------------------------------------------------------------
TARGET = $(BINDIR)/vm 

all: $(TARGET) 

helpers:
	make -f Makefile.helpers

tester: $(SRCDIR)/test_schedule.c
	${CC} $(CFLAGS) -o $@ $^ $(OBJDIR)/vm_support.o $(OBJDIR)/vm_schedule.o

# Links the object files to create the target binary
$(TARGET): $(OBJS) $(INCDIR)
	${CC} ${CFLAGS} -o $@ $(OBJS) -lvm_sd; make -f Makefile.helpers

#$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c 
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/vm_settings.h					 
	${CC} $(CFLAGS) -c -o $@ $<

#--------------------------------------------------------------------
# Cleans the binaries
#--------------------------------------------------------------------
clean:
	rm -f $(SRCOBJS) $(TARGET) $(HELPER_TARGETS)
	rm -f tester

cleanall:
	rm -f $(SRCOBJS) $(TARGET) $(HELPER_TARGETS)
	make -f Makefile.helpers clean

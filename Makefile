CXX = u++				# compiler
CXXFLAGS = -g -Wall -Wextra -MMD -Wno-implicit-fallthrough # compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}# makefile name

OBJECTS = groupoff.o printer.o truck.o bank.o config.o main.o parent.o bottling_plant.o \
		  name_server.o vending_machine.o wat_card_office.o wat_card.o student.o # list of object files needed
EXEC = soda

DEPENDS = ${OBJECTS:.o=.d}

#############################################################

.PHONY : all clean

all : ${EXEC}				# build all executables

${EXEC} : ${OBJECTS}		# optional build of given program
	${CXX} ${CXXFLAGS} $^ -o $@

#############################################################

${OBJECTS} : ${MAKEFILE_NAME}		# OPTIONAL : changes to this file => recompile

-include ${DEPENDS}			# include *.d files containing program dependences

clean :					# remove files that can be regenerated
	rm -f *.d *.o ${OBJECTS} ${EXEC}

########       AVR Library Makefile       ########
#####                                        #####
#####      Makefile adapté                  #####
#####          pour lib_dir - 2023           #####
##################################################

# Ce Makefile vous permet de compiler des projets
# pour les microcontroleurs Atmel AVR sur 
# Linux ou Unix, en utilisant l'outil AVR-GCC. 
# Ce Makefile supporte C & C++

#####      Details spécifique à la cible       #####
MCU=atmega324pa
PROJECTNAME=libstatique
PRJSRC= LED.cpp Timer.cpp Bouton.cpp Wheel.cpp Navigation.cpp
INC=
LIBS=
OPTLEVEL=s

####################################################

####### variables #######
CC=avr-gcc
OBJCOPY=avr-objcopy
AVRDUDE=avrdude
REMOVE=rm -f
HEXFORMAT=ihex
AR=avr-ar

####### Options de compilation #######
CFLAGS=-I. -MMD $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-std=c++14 -fpack-struct -fshort-enums \
	-funsigned-bitfields -funsigned-char \
	-Wall

GCCVERSION := $(shell expr `$(CC) -dumpversion | cut -f1 -d.` \>= 12)
ifeq "$(GCCVERSION)" "1"
    CFLAGS += --param=min-pagesize=0
endif

CXXFLAGS=-fno-exceptions

####### Cible (Target) #######
TRG=$(PROJECTNAME).a

####### Definition de tout les fichiers objets #######
CFILES=$(filter %.c, $(PRJSRC))
CPPFILES=$(filter %.cpp, $(PRJSRC))
OBJDEPS=$(CFILES:.c=.o) $(CPPFILES:.cpp=.o)

####### Creation des commandes du Makefile #######
.PHONY: all install clean

all: $(TRG)

$(TRG): $(OBJDEPS)
	$(AR) rcs $(TRG) $(OBJDEPS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $<

-include *.d

install: $(TRG)
	@echo "Librairie compilée. "

clean:
	$(REMOVE) $(TRG) $(OBJDEPS) *.d

#####                    EOF                   #####
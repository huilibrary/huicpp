##===================================================================##
## File Name: makefile
## Create Author: Joseph Hui
## Create Date: Tue Sep 12 1359 2015
## Description:
##		
##===================================================================##

include ../inc.mk


TARGET = ${LIB_PATH}/libhuicpp.a

SRC = $(wildcard *.cpp)

OBJS = ${patsubst %.cpp, %.o, ${SRC}}

ifeq "$(OS_TYPE)" "Darwin"
LINK_CC_LIBS += -liconv
endif

all: def

def: ${TARGET}


include ../route.mk

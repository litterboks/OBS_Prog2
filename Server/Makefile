CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++11
EXECUTABLE = gridserver

############################### 
# Build
###############################

all: gridserver.cpp
	${CC} ${CFLAGS} -o ${EXECUTABLE} $^

############################### 
# Misc
###############################

clean:
	rm -f *.zip ${EXECUTABLE}
	find . -name "*.o" -type f -delete

run: all
	./${EXECUTABLE}

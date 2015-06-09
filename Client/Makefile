CC = g++
CFLAGS = -Wall -Werror -pedantic -std=c++11
EXECUTABLE = vehicleclient

############################### 
# Build
###############################

all: vehicleclient.cpp
	${CC} ${CFLAGS} -o ${EXECUTABLE} $^

############################### 
# Misc
###############################

clean:
	rm -f *.zip ${EXECUTABLE}
	find . -name "*.o" -type f -delete

run: all
	./${EXECUTABLE}

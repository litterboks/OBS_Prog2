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

zip: clean
	        zip -r mikovits_if14b062_steinberger_if14b049.zip . -x .git/ -x .gitignore -x .git/**\* -x *.pdf -x *.in -x inputs/


run: all
	./${EXECUTABLE}

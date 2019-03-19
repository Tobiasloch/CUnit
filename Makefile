$(CC)=gcc

# path variable beschreibt Ort
# name variable beschreibt name der variable ohne dateiendung

# CUnit path
unit:=Assertions

# file that contains test data and main function
testFile:=Test/test
# file that is being tested
testedFile:=Test/main

# object folder. pattern: %/
# replace the % with the foldername. If the files should be in the active folder, then you should use an empty string without the terminating /.
objectFolder:=target/src/
testFolder:=target/test/

# list of includes separated with spaces and without file ending
includes:=Test/include1 Test/include2
# compiler flags for include files e.g. -lm
includeCFlags:=

# resources
# list all online or offline ressource urls in the following variable
ressources:=https://github.com/Tobiasloch/CUnit/blob/master/include1.o https://github.com/Tobiasloch/CUnit/blob/master/include2.o

default: help

help:
	$(info The make call should call the following targets: make [validate|compile|test|commit|deploy])
	exit
	
# compiling targets
compile: validate checkObjectFolder checkTestFolder compileIncludes testedFile assertionsFile testFile
	$(info compiling was successful!)
	
# compiler calls
testFile: $(patsubst %,$(objectFolder)%.o,$(notdir $(includes))) $(objectFolder)$(notdir $(testedFile)).o $(testFolder)$(notdir $(unit)).o
	$(CC) $(testFile).c $(objectFolder)*.o $(testFolder)*.o $(includeCFlags) -pthread -o $(notdir $(testFile))

assertionsFile: $(unit).c 
	$(CC) -c $(unit).c -o $(testFolder)$(notdir $(unit)).o -pthread

testedFile: $(testedFile).c
	$(CC) -c $(includeCFlags) $(testedFile).c -o $(objectFolder)$(notdir $(testedFile)).o
	
compileIncludes: $(filter %,$(includes))

$(filter %,$(includes)): %: %.c
	$(CC) -c $@.c $(includeCFlags) -o $(objectFolder)$(notdir $@).o
	
validate:
	$(info validating was successful!)

test: compile
	./$(notdir $(testFile))
	
testClean: test cleanTests
	$(info testing was successful!)

commit:test
	git add $(unit).c $(unit).h $(testFile).c $(testFile).h
	git add $(testedFile).c $(testedFile).h
	git add $(patsubst %,%.c,$(includes)) $(patsubst %,%.h,$(includes))
	git add $(objectFolder)
	git add $(testFolder)
	git add Makefile
	git diff-index --quiet HEAD || git commit
	$(info committing was successful!)
	
deploy: commit
	git push
	$(info deploying was successful!)

checkObjectFolder:
	mkdir -p $(objectFolder)
	
checkTestFolder:
	mkdir -p $(testFolder)

cleanTests:
	rm $(testFolder)*.o $(notdir $(testFile))
	
clean:
	rm $(objectFolder)*.o $(testFolder)*.o $(notdir $(testFile))
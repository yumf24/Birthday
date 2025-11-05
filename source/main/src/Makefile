# just for test

DIRS = objs


all: | $(DIRS)
	g++ -o objs/main.o -c main.cpp
	g++ -o objs/time.o -c time.cpp
	g++ -o objs/birthclass.o -c birthclass.cpp
	g++ -o exe objs/main.o objs/time.o objs/birthclass.o 
	@echo ":) Completed"


$(DIRS):
	mkdir $(DIRS)


clean:
	rm -rf objs
	rm exe
	@echo ":) Completed"


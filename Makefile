.PHONY: debug
debug:
ifeq ($(wildcard debug),)
	mkdir debug
endif
	(cd debug; cmake -G KDevelop3 .. -DCMAKE_BUILD_TYPE=Debug)
	(cd debug; make)

.PHONY: release
release:
ifeq ($(wildcard release),)
	mkdir release
endif
	(cd release; cmake -G KDevelop3 .. -DCMAKE_BUILD_TYPE=Release)
	(cd release; make)

.PHONY: all
all: debug release 

.PHONY: clean
clean: 
	rm -Rf debug
	rm -Rf release

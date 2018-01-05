goal: draco git

draco:
	gcc -g -o draco draco.cpp

git:
	git add draco.cpp draco.h Makefile
	git commit -v -m "made some changes"

clean:
	rm -rf draco

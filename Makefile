goal: draco git

draco:
	g++ -g -o draco draco.cpp

git:
	git add draco.cpp draco.h Makefile README.md
	git commit -v -m "made some changes"

clean:
	rm -rf draco

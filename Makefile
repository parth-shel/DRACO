goal: draco visualizer git

draco:
	g++ -g -o draco draco.cpp

visualizer:
	gcc -g -w -o draco-visualizer draco-visualizer.cpp /usr/lib/libXbgi.a -lX11 -lm

git:
	git add draco.cpp draco.h Makefile README.md
	git commit -v -m "made some changes"

clean:
	rm -rf draco

goal: draco visualizer

draco:
	g++ -g -o draco draco.cpp

visualizer:
	gcc -g -w -o draco-visualizer draco-visualizer.cpp /usr/lib/libXbgi.a -lX11 -lm

clean:
	rm -rf draco
	rm -rf draco-visualizer

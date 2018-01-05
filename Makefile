goal: draco git

draco:
	gcc -g -o draco draco.cpp

git:
	git add draco.cpp
	git commit -v -m "made some changes"

clean:
	rm -rf draco

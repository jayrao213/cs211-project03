build:
	rm -f ./a.out
	g++ -std=c++20 -g -Wall -pedantic -Werror *.cpp -lm -lcurl -Wno-psabi -Wno-unused-variable -Wno-unused-function

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++20 -g -Wall -pedantic -Werror *.cpp -lm -lcurl -Wno-psabi -Wno-unused-variable -Wno-unused-function
	valgrind --tool=memcheck --leak-check=full --track-origins=yes ./a.out

clean:
	rm -f ./a.out

submit:
	/gradescope/gs submit 1130317 6990053 *.cpp *.h

commit:
	git add .
	git commit -m "$(msg)"

push:
	git push origin main

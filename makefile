all:
	g++ -o principal principal.cpp leitor.cpp tank.cpp tinyxml.cpp tinystr.cpp tinyxmlparser.cpp tinyxmlerror.cpp -lGL -lGLU -lglut 
run:
	./principal
clean:
	rm -rf *o principal

ffosh:ffosh.o myShell.o lineParser.o myVar.o
	g++ -std=gnu++11 -Wall -Werror -pedantic ffosh.o myShell.o lineParser.o myVar.o -o ffosh
ffosh.o:ffosh.cpp myShell.h lineParser.h myVar.h
	g++ -std=gnu++11 -Wall -Werror -pedantic -c ffosh.cpp -o ffosh.o -g
myShell.o:myShell.cpp myShell.h lineParser.h myVar.h
	g++ -std=gnu++11 -Wall -Werror -pedantic -c myShell.cpp -o myShell.o -g
lineParser.o:lineParser.cpp lineParser.h
	g++ -std=gnu++11 -Wall -Werror -pedantic -c lineParser.cpp -o lineParser.o -g
myVar.o:myVar.cpp myVar.h lineParser.h
	g++ -std=gnu++11 -Wall -Werror -pedantic -c myVar.cpp -o myVar.o -g
clean:
	rm ffosh ffosh.o myShell.o lineParser.o myVar.o -f

build_run: build run

build:
	g++ main.cpp lexer/cursor.cpp lexer/match.cpp lexer/lexer.cpp gdxlanguage/gdxlexer.cpp -o main

run:
	./main


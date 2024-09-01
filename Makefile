all: main parser commands.bin output.svg output.pdf
main: main.c
	gcc main.c -o main -L ./raylib/lib/ -l:libraylib.a -lm -g

parser: parser.c 
	gcc parser.c -o parser -L ./raylib/lib/ -l:libraylib.a -lm -g

commands.bin: input.txt parser
	./parser input.txt commands.bin

output.svg: main commands.bin
	./main run-svg commands.bin

output.pdf: main commands.bin
	./main run-tikz commands.bin
	pdflatex output.tex

clean:
	rm main parser commands.bin output.svg output.tex output.pdf output.log output.aux

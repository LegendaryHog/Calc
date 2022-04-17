all: compall run


compall:
	gcc lexer/lexer.c Calcul/calc.c Calcul/main.c -lm -o calc.out

run:
	./calc.out

valrun:
	valgrind -s ./calc.out
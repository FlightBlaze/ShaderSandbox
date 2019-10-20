cmake .

echo '.PHONY : run
run : $(PROG)
	./shadersandbox' >> ./Makefile

make

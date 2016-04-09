SRC= \
./nodelist.o\
./exceptions/missingargument.o\
./exceptions/parsersyntax.o\
./exceptions/excep.o\
./exceptions/invalidinit.o\
./exceptions/invalidindex.o\
./exceptions/invalidtype.o\
./exceptions/negativevalue.o\
./exceptions/invalidassignment.o\
./exceptions/dividebyzero.o\
./exceptions/lexersyntax.o\
./node.o\
./varmanager.o\
./outcome.o\
./lexer.o\
./primitives/logical.o\
./primitives/integer.o\
./primitives/real.o\
./primitives/variable.o\
./primitives/procedure.o\
./primitives/text.o\
./primitives/sequence.o\
./primitives/specialfunction.o\
./operations/unequal.o\
./operations/multiply.o\
./operations/remainder.o\
./operations/add.o\
./operations/select.o\
./operations/not.o\
./operations/greater.o\
./operations/less.o\
./operations/negate.o\
./operations/length.o\
./operations/subtract.o\
./operations/intdivide.o\
./operations/slice.o\
./operations/greateq.o\
./operations/call.o\
./operations/divide.o\
./operations/or.o\
./operations/and.o\
./operations/equal.o\
./operations/exponent.o\
./operations/lesseq.o\
./object.o\
./statements/repeat.o\
./statements/return.o\
./statements/extern.o\
./statements/assign.o\
./statements/for.o\
./statements/intern.o\
./statements/sliceassign.o\
./statements/input.o\
./statements/selectassign.o\
./statements/do.o\
./statements/output.o\
./statements/exit.o\
./statements/if.o\
./statements/end.o\
./statements/while.o\
./statements/case.o\
./parser.o\
./token.o

all: rapira
all2: rapira rapiraParser

clean:
	rm -f rapira rapiraParser *.o statements/*.o operations/*.o primitives/*.o exceptions/*.o

%.o: %.cpp
	g++ -c --std=c++11 -o $@ $<

rapira: $(SRC)
	g++ -o rapira rapira.cpp $(SRC)

rapiraParser: $(SRC)
	g++ --std=c++11 -o rapiraParser -DPARSERONLY rapira.cpp $(SRC)

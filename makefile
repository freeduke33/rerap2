SRC= \
./nodelist.cpp\
./exceptions/missingargument.cpp\
./exceptions/parsersyntax.cpp\
./exceptions/excep.cpp\
./exceptions/invalidinit.cpp\
./exceptions/invalidindex.cpp\
./exceptions/invalidtype.cpp\
./exceptions/negativevalue.cpp\
./exceptions/invalidassignment.cpp\
./exceptions/dividebyzero.cpp\
./exceptions/lexersyntax.cpp\
./node.cpp\
./varmanager.cpp\
./outcome.cpp\
./lexer.cpp\
./primitives/logical.cpp\
./primitives/integer.cpp\
./primitives/real.cpp\
./primitives/variable.cpp\
./primitives/procedure.cpp\
./primitives/text.cpp\
./primitives/sequence.cpp\
./primitives/specialfunction.cpp\
./operations/unequal.cpp\
./operations/multiply.cpp\
./operations/remainder.cpp\
./operations/add.cpp\
./operations/select.cpp\
./operations/not.cpp\
./operations/greater.cpp\
./operations/less.cpp\
./operations/negate.cpp\
./operations/length.cpp\
./operations/subtract.cpp\
./operations/intdivide.cpp\
./operations/slice.cpp\
./operations/greateq.cpp\
./operations/call.cpp\
./operations/divide.cpp\
./operations/or.cpp\
./operations/and.cpp\
./operations/equal.cpp\
./operations/exponent.cpp\
./operations/lesseq.cpp\
./object.cpp\
./statements/repeat.cpp\
./statements/return.cpp\
./statements/extern.cpp\
./statements/assign.cpp\
./statements/for.cpp\
./statements/intern.cpp\
./statements/sliceassign.cpp\
./statements/input.cpp\
./statements/selectassign.cpp\
./statements/do.cpp\
./statements/output.cpp\
./statements/exit.cpp\
./statements/if.cpp\
./statements/end.cpp\
./statements/while.cpp\
./statements/case.cpp\
./parser.cpp\
./rapira.cpp\
./token.cpp

all: rapira rapiraParser

rapira: $(SRC)
	g++ -o rapira $(SRC)

rapiraParser: $(SRC)
	g++ -o rapiraParser -DPARSERONLY $(SRC)

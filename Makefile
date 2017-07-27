TARGET = expr

all: ${TARGET}

${TARGET}: Expression.o Statement.o ${TARGET}_p.o ${TARGET}_l.o main.o
	g++ -o $@ $^

Expression.o: Expression.cpp Expression.h
	g++ -g -c -o $@ $<

Statement.o: Statement.cpp Statement.h
	g++ -g -c -o $@ $<

main.o: main.cpp
	g++ -c -o $@ $<

${TARGET}_l.o: ${TARGET}_l.cpp
	g++ -c -o $@ $<

${TARGET}_l.cpp: expr.l
	flex -o $@  $<

${TARGET}_p.o: ${TARGET}_p.cpp
	g++ -c -o $@ $<

${TARGET}_p.cpp: expr.y
	./lemon -p -Tlempar.c $<
	mv ${TARGET}.c $@
	mv ${TARGET}.h tokens.h

clean:
	rm -f *.o
	rm -f ${TARGET}_l.cpp
	rm -f ${TARGET}_l.h
	rm -f ${TARGET}_p.cpp
	rm -f tokens.h
	rm -f ${TARGET}.exe
	rm -f ${TARGET}
	rm -f ${TARGET}.out
	rm -f ${TARGET}.c
	rm -f ${TARGET}.h
test:
	./expr < samples/test1.txt

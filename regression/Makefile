CC=cc
LEX=lex
YACC=yacc
LEX_CFLAGS=-c -D__DEBUG__
CFLAGS=-Wall -D__DEBUG__
LDFLAGS=-ll

LEX_DEF=../src/scanner.l
TARGET=regtest_lex

all: $(TARGET)

$(TARGET): lex.yy.o
	$(CC) $(CFLAGS) lex.yy.o $(LDFLAGS) -o $(TARGET) 

lex.yy.o: lex.yy.c common.h
	$(CC) $(LEX_CFLAGS) lex.yy.c

lex.yy.c: $(LEX_DEF)
	$(LEX) $(LEX_DEF)

test:
	./test_lex
	./test_parse
	./test_sem
clean:
	rm -rf *.o lex.yy.c $(TARGET)

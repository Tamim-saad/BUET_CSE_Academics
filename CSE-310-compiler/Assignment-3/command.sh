yacc -d -y 2005095.y
g++ -w -c -o y.o y.tab.c
flex 2005095.l
g++ -w -c -o l.o lex.yy.c
# if the above command doesn't work try g++ -fpermissive -w -c -o l.o lex.yy.c
g++ y.o l.o -lfl -o 2005095
./2005095 input.txt
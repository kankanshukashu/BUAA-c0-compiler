#！/bin/sh
g++ -c src/main.cpp -o src/main.o
g++ -c src/tokenizer.cpp -o src/tokenizer.o
g++ -c src/analyser.cpp -o src/analyser.o
g++ -c src/generator.cpp -o src/generator.o
g++ src/main.o src/tokenizer.o src/analyser.o src/generator.o -o cc0
./cc0 -h

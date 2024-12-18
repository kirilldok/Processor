.PHONY: all run

all: main


WARNINGFLAGS := -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -Wstack-usage=8192 -Werror=vla

SANITIZEFLAGS := -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


CXX := g++
CPPFLAGS := -D _DEBUG
CXXFLAGS := -ggdb3 -std=c++17 -O0 -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer  -pie -fPIE $(WARNINGFLAGS) $(SANITIZEFLAGS)
LDFLAGS := -pie -fPIE $(SANITIZEFLAGS)

Objects/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -I ./MyStack $< -c -o $@

SOURCES := Objects/ProcessorFunc.o Objects/LableTable.o MyStack/stack.a

main: Objects/Processor.o $(SOURCES)
	$(CXX) Objects/Processor.o $(SOURCES) $(LDFLAGS) -o main

compiler: Objects/PROGRAMM_ASM.o Objects/Asm_Commands.o $(SOURCES)
	$(CXX) Objects/PROGRAMM_ASM.o Objects/Asm_Commands.o $(SOURCES) $(LDFLAGS) -o compiler


run: main
	./main

compile: compiler
	./compiler

clean:
	rm Objects/*.o
	rm main

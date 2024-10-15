.PHONY: all run

all: main

WARNINGFLAGS := -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -Wlarger-than=8192 -Wstack-usage=8192 -Werror=vla

SANITIZEFLAGS := -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


CXX := g++
CPPFLAGS := -D _DEBUG
CXXFLAGS := -ggdb3 -std=c++17 -O0 -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer  -pie -fPIE $(WARNINGFLAGS) $(SANITIZEFLAGS)
LDFLAGS := -pie -fPIE $(SANITIZEFLAGS)

Objects/%.o : %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -c -o $@

SOURCES :=  Objects/StackFunc.o Objects/Stackdebug.o Objects/hash.o

main: Objects/main.o $(SOURCES)
	$(CXX) Objects/main.o $(SOURCES) $(LDFLAGS) -o main

compiler: Objects/PROGRAMM_ASM.o $(SOURCES)
	$(CXX) Objects/PROGRAMM_ASM.o $(SOURCES) $(LDFLAGS) -o compiler


run: main
	./main

compile: compiler
	./compiler

clean:
	rm Objects/*.o
	rm main


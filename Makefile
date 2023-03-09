ifeq ($(OS)),Windows_NT)

WIN_COMPILER=cl.exe
WIN_COMPILER_DEBUG_FLAGS=/Zi /EHsc /nologo /std:c++20 /W4 /WX /fsanitize=address

wintest: obj/testing_main.windebug.obj obj/ntest.windebug.obj obj/lexer.windebug.obj obj/util.windebug.obj obj/term.windebug.obj
	$(WIN_COMPILER) $(WIN_COMPILER_DEBUG_FLAGS) $^ /Fe"testing.exe" /Fo"obj"
	./testing.exe

obj/testing_main.windebug.obj: src/testing_main.cpp
	$(WIN_COMPILER) $(WIN_COMPILER_DEBUG_FLAGS) /c src/testing_main.cpp /Fo"obj/testing_main.windebug.obj"

obj/ntest.windebug.obj: src/ntest.cpp src/ntest.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_DEBUG_FLAGS) /c src/ntest.cpp /Fo"obj/ntest.windebug.obj"

obj/lexer.windebug.obj: src/lexer.cpp src/lexer.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_DEBUG_FLAGS) /c src/lexer.cpp /Fo"obj/lexer.windebug.obj"

obj/util.windebug.obj: src/util.cpp src/util.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_DEBUG_FLAGS) /c src/util.cpp /Fo"obj/util.windebug.obj"

obj/term.windebug.obj: src/term.cpp src/term.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_DEBUG_FLAGS) /c src/term.cpp /Fo"obj/term.windebug.obj"

else

LINUX_COMPILER=g++
LINUX_COMPILER_DEBUG_FLAGS=-g -std=c++20 -Wall -Wpedantic -Wextra -Wconversion -Werror

default:
	mkdir -p obj/
	make lxtest

clean:
	rm -r -f obj/ testing.elf

lxtest: obj/testing_main.lxdebug.o obj/ntest.lxdebug.o obj/lexer.lxdebug.o obj/util.lxdebug.o obj/term.lxdebug.o
	$(LINUX_COMPILER) $(LINUX_COMPILER_DEBUG_FLAGS) $^ -o testing.elf
	./testing.elf

obj/testing_main.lxdebug.o: src/testing_main.cpp
	$(LINUX_COMPILER) $(LINUX_COMPILER_DEBUG_FLAGS) -c src/testing_main.cpp -o obj/testing_main.lxdebug.o

obj/ntest.lxdebug.o: src/ntest.cpp src/ntest.hpp
	$(LINUX_COMPILER) $(LINUX_COMPILER_DEBUG_FLAGS) -c src/ntest.cpp -o obj/ntest.lxdebug.o

obj/lexer.lxdebug.o: src/lexer.cpp src/lexer.hpp
	$(LINUX_COMPILER) $(LINUX_COMPILER_DEBUG_FLAGS) -c src/lexer.cpp -o obj/lexer.lxdebug.o

obj/util.lxdebug.o: src/util.cpp src/util.hpp
	$(LINUX_COMPILER) $(LINUX_COMPILER_DEBUG_FLAGS) -c src/util.cpp -o obj/util.lxdebug.o

obj/term.lxdebug.o: src/term.cpp src/term.hpp
	$(LINUX_COMPILER) $(LINUX_COMPILER_DEBUG_FLAGS) -c src/term.cpp -o obj/term.lxdebug.o

endif

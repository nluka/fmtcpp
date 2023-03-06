WIN_COMPILER=cl.exe
WIN_COMPILER_FLAGS=/Zi /EHsc /nologo /std:c++20 /W4 /WX /fsanitize=address

win: obj/main.obj obj/ntest.obj obj/lexer.obj obj/util.obj obj/term.obj
	$(WIN_COMPILER) $(WIN_COMPILER_FLAGS) $^ /Fe"testing.exe" /Fo"obj"
	./testing.exe

obj/main.obj: main.cpp
	$(WIN_COMPILER) $(WIN_COMPILER_FLAGS) /c main.cpp /Fo"obj/main.obj"

obj/ntest.obj: ./ntest/ntest.cpp ./ntest/ntest.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_FLAGS) /c ./ntest/ntest.cpp /Fo"obj/ntest.obj"

obj/lexer.obj: ../src/lexer.cpp ../src/lexer.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_FLAGS) /c ../src/lexer.cpp /Fo"obj/lexer.obj"

obj/util.obj: ../src/util.cpp ../src/util.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_FLAGS) /c ../src/util.cpp /Fo"obj/util.obj"

obj/term.obj: ../src/term.cpp ../src/term.hpp
	$(WIN_COMPILER) $(WIN_COMPILER_FLAGS) /c ../src/term.cpp /Fo"obj/term.obj"

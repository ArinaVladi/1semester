# Applicaton options
GXX_STANDARD = 17 

# Сompiler options
CC = g++
CXXFLAGS = -Wall -Wextra -std=c++$(GXX_STANDARD)
#CXXFLAGS += -g -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Waggressive-loop-optimizations -Walloc-zero -Walloca -Walloca-larger-than=8192 -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wdangling-else -Wduplicated-branches -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Winline -Wlarger-than=8192 -Wvla-larger-than=8192 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wstringop-overflow=4 -Wsuggest-attribute=noreturn -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs -Waligned-new -Walloc-size-larger-than=1073741824 -Walloc-zero -Walloca -Walloca-larger-than=8192 -Wcast-align=strict -Wdangling-else -Wduplicated-branches -Wformat-overflow=2 -Wformat-truncation=2 -Wmissing-attributes -Wmultistatement-macros -Wrestrict -Wshadow=global -Wsuggest-attribute=malloc -fcheck-new -fsized-deallocation -fstack-check -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer
LXXFLAGS =
BUILD = Debug# Debug or Release

# Folder name setting
SRC_DIR = src
BIN_DIR = bin

# Setting build parameters
ifeq ($(BUILD), Debug)
    CXXFLAGS += -O0 -g -fdiagnostics-color=always
    BUILD_PATH = build_debug
else
    CXXFLAGS += -Os -s -DNDEBUG
    BUILD_PATH = build-release
endif

LXXFLAGS += -lgtest -lgtest_main -lpthread

# Search for source files
SRC_FULL_PATH = $(shell where /r ./ *.cpp)
SRC = src
OBJ = $(addprefix $(BIN_DIR)/, $(SRC:.cpp=.o))


# Build project
list: bin/list.o bin/tests.o bin/log.o Makefile
	g++ bin/list.o bin/tests.o bin/log.o -o build_debug/list $(LXXFLAGS)

bin/list.o: src/list.cpp Makefile
	$(CC) $< -c -o $@ $(CXXFLAGS)

bin/tests.o: src/tests.cpp Makefile
	$(CC) $< -c -o $@ $(CXXFLAGS) $(LXXFLAGS)

bin/log.o: src/log.cpp Makefile
	$(CC) $< -c -o $@ $(CXXFLAGS)	

.PHONY: init clean
init:
	md bin

clean:
	rm -rf bin/*
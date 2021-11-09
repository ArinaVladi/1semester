# Applicaton options
#APPLICATION = $(notdir $(CURDIR)) # Application name is name of root holder
GXX_STANDARD = 17 # 11, 14, 17, 20
#OUT_FILE_NAME = $(APPLICATION)
#ASSM_OUT_FILE_NAME = assm

# Сompiler options
CC = g++
CXXFLAGS = -Wall -Wextra -std=c++$(GXX_STANDARD)
CXXFLAGS += -g -Wc++0x-compat -Wc++11-compat -Wc++14-compat -Waggressive-loop-optimizations -Walloc-zero -Walloca -Walloca-larger-than=8192 -Warray-bounds -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wdangling-else -Wduplicated-branches -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Winline -Wlarger-than=8192 -Wvla-larger-than=8192 -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls -Wrestrict -Wshadow -Wsign-promo -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2 -Wstringop-overflow=4 -Wsuggest-attribute=noreturn -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wnarrowing -Wno-old-style-cast -Wvarargs -Waligned-new -Walloc-size-larger-than=1073741824 -Walloc-zero -Walloca -Walloca-larger-than=8192 -Wcast-align=strict -Wdangling-else -Wduplicated-branches -Wformat-overflow=2 -Wformat-truncation=2 -Wmissing-attributes -Wmultistatement-macros -Wrestrict -Wshadow=global -Wsuggest-attribute=malloc -fcheck-new -fsized-deallocation -fstack-check -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer
LXXFLAGS =
BUILD = Debug# Debug or Release

# Folder name setting
#SRC_DIR = src
#BIN_DIR = bin

# Setting build parameters
ifeq ($(BUILD), Debug)
    CXXFLAGS += -O0 -g -fdiagnostics-color=always
    BUILD_PATH = build-debug
else
    CXXFLAGS += -Os -s -DNDEBUG
    BUILD_PATH = build-release
endif


# Search for source files
#SRC_FULL_PATH = $(shell where /r .\ *.cpp)
#EXLUDED = # Excluded files
#SRC = $(filter-out $(EXLUDED),$(notdir $(SRC_FULL_PATH)))
#OBJ = $(addprefix $(BIN_DIR)/, $(SRC:.cpp=.o))

# Include library
#LIB_PATH = D:/MainFolder/SFML-2.5.1
#LIB_DEPEND = sfml-graphics-d sfml-window-d sfml-system-d
#CXXFLAGS += $(addprefix -I, $(LIB_PATH)/include)
#LXXFLAGS += $(addprefix -L, $(LIB_PATH)/lib) $(addprefix -l, $(LIB_DEPEND))


CXXFLAGS += -Isrc/include
LXXFLAGS += -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system


#
#VPATH = $(subst \,/,$(dir $(SRC_FULL_PATH)))




default: assembler processor

assembler: Makefile assembler/assembler.o constants.h another_constants.h enums_and_constants.h macro_util.h utilities.h utilities.cpp
	g++ assembler/assembler.o -o assm 

assembler.o: assembler/assembler.cpp constants.h another_constants.h enums_and_constants.h macro_util.h utilities.h utilities.cpp
	g++ -c assembler/assembler.cpp $(CXXFLAGS)


processor: Makefile run.o processor/cpu.o processor/stack.o processor/dump.o processor/fixed_point.o processor/listing.o constants.h another_constants.h enums_and_constants.h macro_util.h utilities.h utilities.cpp
	g++ run.o processor/cpu.o processor/dump.o processor/fixed_point.o processor/listing.o processor/stack.o -o cpu  $(LXXFLAGS)

cpu.o: processor/cpu.cpp 
	g++ -c processor/cpu.cpp $(CXXFLAGS) 

dump.o: processor/dump.cpp
	g++ -c 	processor/dump.cpp $(CXXFLAGS) 

fixed_point.o: processor/fixed_point.cpp fixed_point.h
	g++ -c 	processor/fixed_point.cpp $(CXXFLAGS) 

listing.o: processor/listing.cpp
	g++ -c 	processor/listing.cpp $(CXXFLAGS) 

stack.o: processor/stack.cpp macro_util.h
	g++ -c 	processor/stack.cpp $(CXXFLAGS) 	

run.o: run.cpp
	g++ -c run.cpp $(CXXFLAGS) 	



.PHONY: init clean
init:
	md bin

clean:
	rmdir /S /Q bin
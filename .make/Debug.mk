COMPILER:=x86_64-w64-mingw32-g++-win32
ROOT:=$(CURDIR)
BIN:=bin
SRC:=src
BUILD:=Debug
NAME:=MatrixTest
ARG:=

INCLUDE:=\
$(ROOT)/$(SRC)

LIBRARY:=
LIBNAME:=

LFLAG:=\
-static

CFLAG:=\
-D_DEBUG \
-std=gnu++2a -fconcepts\
-g -Og \
-Wall -Werror -Wpedantic \
-Wno-error=pedantic \
-Wno-error=unknown-pragmas \
-Wno-error=unused-variable \
-Wno-error=unused-but-set-variable \

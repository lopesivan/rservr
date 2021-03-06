#!/bin/sh

cd "$( dirname "$0" )"

bison -Wall -b ipc-parser -y -d ipc-parser.y && \
  { mv -v ipc-parser.tab.c ipc-parser.tab.cpp;
    mv -v ipc-parser.tab.h ../../include/protocol/ipc;
    gsed -i 's/ipc-parser\.tab\.c/ipc-parser.tab.cpp/g' ipc-parser.tab.cpp; }

reflex -o ipc-lexer.c ipc-lexer.l

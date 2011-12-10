#!/bin/bash

mv flex.c flex.f
mv bison.c bison.y

bison -d bison.y
flex flex.f

gcc parser.c bison.tab.c lex.yy.c

mv flex.f flex.c
mv bison.y bison.c

rm bison.tab.c
rm bison.tab.h
rm lex.yy.c

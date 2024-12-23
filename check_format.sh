#!/bin/bash

BOLD='\033[1m'
NORMAL='\033[0m'

DEF='\033[0;39m'
LBLUE='\033[1;34m'

echo -en "\n${LBLUE}${BOLD} CPPCHECK is starting...\n ${DEF}${NORMAL}\n"
find . \
  -not -path './install*'\
  -not -path './build*'\
  -regex '.*/.*\.\(c\)$' \
  -exec cppcheck -v --enable=all\
  --suppress=missingIncludeSystem\
  --suppress=checkersReport\
  --suppress=unmatchedSuppression\
  --suppress=constParameterPointer\
  {} \;

echo -en "\n${LBLUE}${BOLD} Clang Formatter is starting...\n ${DEF}${NORMAL}\n"
find . \
  -not -path './install*'\
  -not -path './build*'\
  -regex '.*/.*\.\(c\|cpp\|h\)$'\
  -exec clang-format --style=Chromium -i --verbose\
  {} \;

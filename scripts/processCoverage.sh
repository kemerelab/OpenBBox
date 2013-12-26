#!/bin/bash

echo "[-] File parttern: $1"
echo "[-] Reading coverage.info"
lcov -d . -c -o coverage.info
echo "[-] Reading src"
lcov --list-full-path -e coverage.info ${1} -o coverage-stripped.info
echo "[-] Creating coverage hmtl"
genhtml -o html-coverage coverage-stripped.info
lcov -d . -z
echo "[-] Done"
exit 0


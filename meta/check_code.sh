#!/bin/sh

set -e

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "${script_path}/.." || exit 1
pwd | grep -q '.git' && cd ..

echo 'Beginning code formatting check.'

CLANG_FORMAT=false
if command -v clang-format-15 >/dev/null 2>&1 ; then
  CLANG_FORMAT=clang-format-15
elif command -v brew >/dev/null 2>&1 && command -v "$(brew --prefix llvm@15)"/bin/clang-format >/dev/null 2>&1 ; then
  CLANG_FORMAT="$(brew --prefix llvm@15)"/bin/clang-format
elif command -v $TOOLCHAIN_DIR/clang-format >/dev/null 2>&1 && $TOOLCHAIN_DIR/clang-format --version | grep -qF ' 15.' ; then
  CLANG_FORMAT=$TOOLCHAIN_DIR/clang-format
elif command -v clang-format >/dev/null 2>&1 ; then
  CLANG_FORMAT=clang-format
  if ! "${CLANG_FORMAT}" --version | awk '{ if (substr($NF, 1, index($NF, ".") - 1) < 15) exit 1; }'; then
    echo "You are using '$("${CLANG_FORMAT}" --version)', which appears to not be clang-format 15 or later."
    echo "It is very likely that the resulting changes are not what you wanted."
  fi
else
  echo "clang-format-15 is not available, but C files need linting! Either skip this script, or install clang-format-15."
  echo "(If you install a package 'clang-format', please make sure it's version 15 or later.)"
  exit 1
fi

echo "Using ${CLANG_FORMAT}"

"$CLANG_FORMAT" --style=file -i $(find src/ -type f -name \*.c -or -name \*.h)


#!/bin/sh

script_path=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
cd "${script_path}/.." || exit 1

mkdir -p .git/hooks
cp meta/{commit-msg,pre-commit} .git/hooks/.


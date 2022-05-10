#!/bin/bash
llvm_version="$(clang --version | awk '/[0-9]+\.[0-9]+\.[0-9]+/{print $3}' | awk -F"." '{print $1}')"
exec llvm-cov-$llvm_version gcov "$@"

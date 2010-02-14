#!/bin/sh

X_FILES="$(< _auto-made_files.txt)"
#echo "X_FILES='${X_FILES}'"

for f in ${X_FILES} ; do
  if [ ! -e "$f" ] ; then
    echo "WARNING: $f file is missing"
  else
    echo "rm -rf $f"
    rm -rf $f
  fi
done

X_FILES="$(find . -name Makefile.in)"
#echo "X_FILES='${X_FILES}'"

for f in ${X_FILES} ; do
  echo "rm -rf $f"
  rm -rf $f
done

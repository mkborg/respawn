#!/bin/bash -eu

echo "-------------------------------------------------------------------------------"
# generate "configure.scan" to be used as template for "configure.ac"
X_CMD="autoscan"
echo "---[${X_CMD}]---"
${X_CMD}

echo "-------------------------------------------------------------------------------"
# genereate "aclocal.m4" necessary for "autoconf" tool
X_CMD="aclocal"
echo "---[${X_CMD}]---"
${X_CMD}

echo "-------------------------------------------------------------------------------"
# generate "configure"
X_CMD="autoconf"
echo "---[${X_CMD}]---"
${X_CMD}

echo "-------------------------------------------------------------------------------"
# generate "config.h.in"
X_CMD="autoheader"
echo "---[${X_CMD}]---"
#${X_CMD}

echo "-------------------------------------------------------------------------------"
X_CMD=""
echo "---[${X_CMD}]---"

# if "AM_INIT_AUTOMAKE" in "configure.ac" doesn't contain "foreign" keyword then mandatory files must be present
#X_MANDATORY_FILES=""
X_MANDATORY_FILES="NEWS README AUTHORS ChangeLog"

# create missing mandatory files
for f in ${X_MANDATORY_FILES} ; do
    if [ ! -f $f ] ; then
	echo "  touch $f"
	touch $f
    else
	echo "  '$f' already exists thus doing nothing"
    fi
done

echo "-------------------------------------------------------------------------------"
X_CMD="automake --add-missing"
echo "---[${X_CMD}]---"
${X_CMD}

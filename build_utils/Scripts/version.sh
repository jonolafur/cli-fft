#!/bin/bash

# Requires three directories:
# $1: Directory in which svnversion is to be run
# $2: Directory of the template file
# $3: Destination of the ${VER_FILE}

VER_FILE=version.h
VERS=`git -C $1 describe --all --long --dirty`
BUILD_DATE=`date`

#sed -e s/'\$GIT_DESCRIBE\$'/"$VERS"/g \
#    -e s/'\$WCNOW\$'/"$BUILD_DATE"/g $2/version.h.tmpl > $3/${VER_FILE} 

sed -e s/'\$GIT_DESCRIBE\$'/'$VERS'/g $2/version.h.tmpl > $3/${VER_FILE}



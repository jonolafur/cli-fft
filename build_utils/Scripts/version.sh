#!/bin/bash

# Requires three directories:
# $1: Directory in which svnversion is to be run
# $2: Directory of the template file
# $3: Destination of the ${VER_FILE}

VER_FILE=version.h
VERS=`svnversion $1`
BUILD_DATE=`date`

sed -e s/'\$WCRANGE\$\$WCMODS?M:\$'/"$VERS"/g \
    -e s/'\$WCNOW\$'/"$BUILD_DATE"/g $2/version.h.tmpl > $3/${VER_FILE} 



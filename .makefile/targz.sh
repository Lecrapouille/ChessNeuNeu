#!/bin/bash

#set -x

HERE=${1##*/}
DATE='-'`date +%Y-%m-%d`
#VERSION='-'`cat VERSION`
NTH=0
TARGET_TGZ="ChessNeuNeu$DATE-v$NTH.tar.gz"

while [ -e "$TARGET_TGZ" ];
do
    NTH=`echo "$TARGET_TGZ" | cut -d"v" -f2 | cut -d"." -f1`
    if [ "$NTH" == "" ];
    then
        echo "ERROR: cannot manage this case $TARGET_TGZ"
        exit 1
    else
        NTH=$(( NTH + 1 ))
        TARGET_TGZ="ChessNeuNeu$DATE-v$NTH.tar.gz"
    fi
done

echo -e "\033[35m*** Tarball:\033[00m \033[36m$TARGET_TGZ\033[00m <= \033[33m$1\033[00m"

cd .. > /dev/null
tar --transform s/ChessNeuNeu/ChessNeuNeu$VERSION/ \
    --exclude='.git' --exclude="ChessNeuNeu-*.tar.gz" --exclude="doc/coverage" \
    --exclude "doc/coverage" --exclude "*/build" -czvf /tmp/$TARGET_TGZ $HERE \
    > /dev/null && mv /tmp/$TARGET_TGZ $HERE

#FIXME OK it's ugly
cd - > /dev/null
cp $TARGET_TGZ ChessNeuNeu$VERSION.tar.gz

#!/bin/bash

# Please specify your qmake path here
QMAKE_PATH="/usr/lib/qt5/bin"

QMAKE_EXEC=$QMAKE_PATH/qmake
BUILD_DIR=$PWD/build

echo "Start building"

if [ ! -f $QMAKE_EXEC ]; then
  echo "QMAKE_PATH is not properly specified. Exit"
  exit 1
fi

echo "qmake path: ${QMAKE_PATH}"

if [ ! -d $BUILD_DIR ]; then
  echo "Create build directory: ${BUILD_DIR}"
  mkdir build
fi

cd Match3Plugin
cp start_values.json ../

cd ../build

echo "Building plugin ..."
$QMAKE_EXEC ../Match3Plugin 2>&1 >/dev/null
make -j4 2>&1 >/dev/null

echo "Building application ..."
$QMAKE_EXEC ../GameMatch3 2>&1 >/dev/null
make -j4 2>&1 >/dev/null

echo "Done"

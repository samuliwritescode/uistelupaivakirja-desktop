#!/bin/sh
qmake -spec macx-g++ -config release
rm -rf Uistelupaivakirja.app
make clean
make
macdeployqt Uistelupaivakirja.app -dmg

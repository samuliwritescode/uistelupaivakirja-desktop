#!/bin/sh
qmake -spec macx-g++ -config release
rm -rf Uistelupaivakirja.app
rm -rf Uistelupaivakirja.dmg
make clean
make
macdeployqt Uistelupaivakirja.app -dmg

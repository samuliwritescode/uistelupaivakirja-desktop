#!/bin/sh
qmake -makefile -spec macx-g++ -config release Uistelupaivakirja.pro
rm -rf Uistelupaivakirja.app
rm -rf Uistelupaivakirja.dmg
make clean
make
macdeployqt Uistelupaivakirja.app -dmg

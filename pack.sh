#!/bin/bash

APPPATH="build/app"
DEBPATH="$APPPATH/DEBIAN"
BINNAME="shaderlabs"
VERSION=$1
BINPATH=usr/shaderlabs/shaderlabs-$VERSION/

cat > "$DEBPATH/postinst" << EOM
ln -sf /$BINPATH/$BINNAME /usr/$BINNAME
EOM

chmod +x $DEBPATH/postinst

mkdir -p $APPPATH/$BINPATH

mv $APPPATH/bin $APPPATH/$BINPATH/
mv $APPPATH/lib $APPPATH/$BINPATH/

dpkg-deb --build $APPPATH
EXC=$?

mv app.deb $BINNAME\_$VERSION.deb

exit $EXC
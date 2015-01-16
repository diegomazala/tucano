#!/bin/sh
# Automatically setup the documentation directory, so you can run `make webdoc` later on.

msg() {
  echo "--> $*"
}

CURRENTDIR="$(pwd)"
TUCANODIR="$(dirname "$(readlink -f "../$0")")"
WEBDOCDIR="${TUCANODIR}/doc/html"
echo $TUCANODIR

msg "Removing $WEBDOCDIR"
rm -rf "$WEBDOCDIR"

msg "Cloning the tucano repository on $WEBDOCDIR"
git clone https://github.com/LCG-UFRJ/tucano.git "$WEBDOCDIR"

msg "Checking out gh-pages and removing the master branch"
cd "$WEBDOCDIR" && git checkout gh-pages && git branch -D master && cd "$CURRENTDIR"


#!/bin/sh
# Automatically setup the documentation folder, so you can run `make webdoc` later.

confirm() {
  # call with a prompt string or use a default one
  read -r -p "${1:-Are you sure? [y/N]} " response
  case $response in
    [yY][eE][sS]|[yY]) 
      true
      ;;
    *)
      false
      ;;
  esac
}

msg() {
  echo "--> $*"
}

CURRENTDIR="$(pwd)"
TUCANODIR="$(dirname "$(readlink -f "../$0")")"
WEBDOCDIR="${TUCANODIR}/doc/html"
echo $TUCANODIR

msg "Removing $WEBDOCDIR"
confirm "Is this right [y/N]?" && rm -rf "$WEBDOCDIR" || exit 0

msg "Cloning the tucano repository on $WEBDOCDIR"
git clone https://github.com/LCG-UFRJ/tucano.git "$WEBDOCDIR"

msg "Checking out gh-pages and removing the master branch"
cd "$WEBDOCDIR" && git checkout gh-pages && git branch -D master && cd "$CURRENTDIR"


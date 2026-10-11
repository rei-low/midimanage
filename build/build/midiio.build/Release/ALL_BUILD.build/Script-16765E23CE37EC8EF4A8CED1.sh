#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  echo Build\ all\ projects
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  echo Build\ all\ projects
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  echo Build\ all\ projects
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  echo Build\ all\ projects
fi


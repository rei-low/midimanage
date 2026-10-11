#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  make -f /Users/brax/max-sdk/source/misc/midiio/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  make -f /Users/brax/max-sdk/source/misc/midiio/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  make -f /Users/brax/max-sdk/source/misc/midiio/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/brax/max-sdk/source/misc/midiio/build
  make -f /Users/brax/max-sdk/source/misc/midiio/build/CMakeScripts/ReRunCMake.make
fi


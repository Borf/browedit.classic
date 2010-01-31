#!/bin/sh
# Line found in Doxygen manual
# http://www.stack.nl/~dimitri/doxygen/config.html#cfg_file_version_filter
svn stat -v $1 | sed -n 's/^[ A-Z?\*|!]\{1,15\}/r/;s/ \{1,15\}/\/r/;s/ .*//p'

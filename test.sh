#!/bin/bash

immodules_file=./immodules
gtk-query-immodules-2.0 .libs/*.so > $immodules_file

export GTK_IM_MODULE=hangul32
export GTK_RC_FILES=`pwd`/gtkrc
export GTK2_RC_FILES=`pwd`/gtkrc
export GTK_IM_MODULE_PATH=.libs/
export GTK_IM_MODULE_FILE=$immodules_file
exec ./entry
# gtk-demo
# exec ./xchat
# exec gedit

#!/bin/bash

immodules_file=./gtk.immodules
gtk-query-immodules-2.0 .libs/*.so > $immodules_file

export GTK_IM_MODULE=hangul2
#export GTK_RC_FILES=`pwd`/gtkrc
#export GTK2_RC_FILES=`pwd`/gtkrc
export GTK_IM_MODULE_PATH=.libs/
export GTK_IM_MODULE_FILE=$immodules_file
#export GDK_USE_XFT=1
# exec ./entry
# exec gtk-demo
# exec ./xchat
exec /usr/local/bin/gedit
#exec /usr/local/bin/pan

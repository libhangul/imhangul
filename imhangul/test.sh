#!/bin/bash

immodules_file=./immodules
if [ ! -f $immodules_file ]; then
	gtk-query-immodules-2.0 .libs/*.so > $immodules_file
fi

export GTK_RC_FILES=`pwd`/gtkrc:$HOME/.gtkrc-2.0
export GTK2_RC_FILES=`pwd`/gtkrc:$HOME/.gtkrc-2.0
export GTK_IM_MODULE_PATH=.libs/
export GTK_IM_MODULE_FILE=$immodules_file
exec ./entry
# gtk-demo
# exec ./xchat
# exec gedit

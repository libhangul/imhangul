#!/bin/bash

immodules_file=immodules
if [ ! -f $immodules_file ]; then
	gtk-query-immodules-2.0 .libs/*.so > $immodules_file
fi

export GTK_RC_FILES=./gtkrc
export GTK_IM_MODULE_PATH=.libs/
export GTK_IM_MODULE_FILE=./$immodules_file
exec ./entry -display :1
# gtk-demo
# exec ./xchat
# exec gedit

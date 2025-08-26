#!/bin/bash

LIB_PATH=$(pwd)/tf2.so
PROCID=$(pgrep tf_linux64 | head -n 1)

if [ -z "$PROCID" ]; then
    echo "Please open game"
    exit 1
fi

unload() {
    echo -e "\nUnloading library with handle $LIB_HANDLE"
    
    RC=$(sudo gdb -n --batch -ex "attach $PROCID" \
             -ex "call ((int (*) (void *)) dlclose)((void *) $LIB_HANDLE)" \
             -ex "call ((char * (*) (void)) dlerror)()" \
             -ex "detach" 2> /dev/null | grep -oP '\$2 = 0x\K[0-9a-f]+')

    if [[ "$RC" == "0" ]]; then
        echo "Library RC unloaded successfully"
    else
        echo "Failed to unload library RC"
    fi
}

trap unload SIGINT

LIB_HANDLE=$(sudo gdb -n --batch -ex "attach $PROCID" \
                  -ex "call ((void * (*) (const char*, int)) dlopen)(\"$LIB_PATH\", 1)" \
                  -ex "detach" 2> /dev/null | grep -oP '\$1 = \(void \*\) \K0x[0-9a-f]+'
	  )

if [ -z "$LIB_HANDLE" ]; then
    echo "Failed to load library $LIB_HANDLE"
    exit 1
fi

echo "Library loaded successfully at $LIB_HANDLE. Use Ctrl+C to unload."

tail -f /tmp/tf2.log

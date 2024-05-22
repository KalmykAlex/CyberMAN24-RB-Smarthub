#!/bin/bash
cd ..

if pidof dvsa >/dev/null; then
	pid=$(pidof dvsa)

	for fd in $(ls /proc/$pid/fd); do
        	eval "exec $fd>&-"
	done

	kill -9 $pid
	echo "$pid: dvsa process killed."
fi

rm -rf build/
mkdir build 2> /dev/null
cd build/
cmake ..
make
if [ $# -eq 0 ]; then
    make run
fi
   

if [ "$1" == "-w" ]; then
	make win CXX=i686-w64-mingw32-g++ #SUFIX=.lib
	exit
fi

make dev

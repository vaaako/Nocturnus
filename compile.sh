if [ "$1" == "-w" ]; then
	make clean
	make win CXX=i686-w64-mingw32-g++ #SUFIX=.lib
	exit
fi

make clean
make dev

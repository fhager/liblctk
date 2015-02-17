#H#liblctk
#H#=======
#H#
#H#Author:		Florian Hager
#H#License:	BSD 2-clause
#H#
#H#Available Makefile targets are:

#H#  help		this help
help:
	@sed -n 's/^#H#//p' < Makefile

#H#  all		all of the below
all: liblctkso liblctkjs liblctkchai

#H#  liblctkso	compile to dynamic c++ library
liblctkso: liblctk.hpp liblctk.cpp
	g++ -std=c++11 -shared -fPIC -o ./liblctk.so liblctk.cpp

#H#  liblctkjs	compile to javascript library
liblctkjs: liblctk.hpp liblctk.cpp liblctk-binds.cpp
	em++ -std=c++11 --bind -s NO_EXIT_RUNTIME=1 -o ./liblctk.js -O2 liblctk-binds.cpp

#H#  liblctkchai	compile to dynamic chaiscript library
liblctkchai: liblctk.hpp liblctk.cpp liblctk-binds.cpp
	g++ -std=c++11 -shared -D __CHAISCRIPT__ -fPIC -o ./liblctk.chai.so liblctk-binds.cpp

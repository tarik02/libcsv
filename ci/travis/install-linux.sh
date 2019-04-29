wget $CMAKE_URL
mkdir -p $CMAKE_DIR
tar --strip-components=1 -xzf $CMAKE_TAR -C $CMAKE_DIR


if [ "$TARGET_CPU" != "x86" ]; then
	sudo apt-get -qq update
else
	sudo dpkg --add-architecture i386
	sudo apt-get -qq update

	# install g++-multilib -- in the end, after i386 packages!
	sudo apt-get install -y g++-multilib
fi

if [ "$GET_COVERAGE" != "" ]; then
	sudo apt-get install -y lcov
fi

exit 0 # ignore any errors

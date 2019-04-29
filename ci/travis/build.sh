THIS_DIR=`pwd`

mkdir build
pushd build
cmake .. -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=$BUILD_CONFIGURATION -DBUILD_COVERAGE=$GET_COVERAGE
cmake --build .
ctest --output-on-failure

popd

if [ "$GET_COVERAGE" != "" ]; then
	lcov --capture --directory . --no-external --output-file coverage.info
	lcov --list coverage.info

	curl -s https://codecov.io/bash | bash
fi

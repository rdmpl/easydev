all:
	-cmake -E make_directory "build"
	-cmake -E chdir "build" cmake -DEASYDEV_ENABLE_TESTING=1 -DBENCHMARK_ENABLE_INSTALL=1 -DCMAKE_BUILD_TYPE=Release ../
	cmake --build "build" --config Release
ctest:
	cmake -E chdir "build" ctest --build-config Release
install:
	@echo "This operation will install the test libriries globally"
	@sudo cmake --build "build" --config Release --target install
clean:
	@rm -rf build

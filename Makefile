all:
	-cmake -E make_directory "build"
	-cmake -E chdir "build" cmake -DBENCHMARK_ENABLE_INSTALL=1 -DCMAKE_BUILD_TYPE=Release ../
	cmake --build "build" --config Release
test:
	cmake -E chdir "build" ctest --build-config Release
install:
	@echo "This operation will install the test libriries globally"
	@sudo cmake --build "build" --config Release --target install
clean:
	@rm -rf build

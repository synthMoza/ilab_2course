#include "BitonicSort.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

using namespace se;

bool BitonicSort::isDeviceSuitable(cl::Device device) {
	return device.getInfo<CL_DEVICE_AVAILABLE>() &&
		device.getInfo<CL_DEVICE_COMPILER_AVAILABLE>();
}

cl::Device BitonicSort::get_device() {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	for (auto platform : platforms) {
		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		for (auto device : devices)
			if (isDeviceSuitable(device))
				return device;
	}
}

cl::Program BitonicSort::getProgram() {
	cl::Program::Sources sources;

	// Read source code from the file
	std::ifstream file{ KERNEL_NAME };
	std::string code;
	if (file) {
		std::ostringstream contents;
		contents << file.rdbuf();
		code = contents.str();
	}
	file.close();
	sources.push_back({ code.c_str(), code.length() });

	cl::Program program(context_, sources);
	if (program.build({device_})) {
		std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_) << std::endl;
		throw std::runtime_error("Couldn't compile program!");
	}

	return program;
}
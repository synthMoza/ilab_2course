#pragma once

#include <vector>

#include <CL/cl.hpp>

namespace se {
	// Class for sorting arrays using bitonic sort and GPU computations
	class BitonicSort {
		const char* KERNEL_NAME = "kernels/kernel.cl";

		cl::Device device_;
		cl::Context context_;
		cl::Program program_;
		cl::CommandQueue queue_;

		/* Iternal methods for constructing the application */
		// Choose compatible device for calculations
		bool isDeviceSuitable(cl::Device device);
		cl::Device get_device();
		// Open source code and create program
		cl::Program getProgram();

	public:
		BitonicSort() : device_(get_device()), context_({device_}) , program_ (getProgram()),
			queue_(context_, device_) {

		}

		// No interface for now...
		void sort() {

		}
	};
}
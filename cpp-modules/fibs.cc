#define BULDING_NODE_EXTENSION
#include <node.h>

#include <stdio.h>
#include <assert.h>
#include <sys/sysctl.h>
#include <sys/stat.h>
#include <stdlib.h>

#include <OpenCL/OpenCL.h>

using namespace v8;

#pragma mark -
#pragma mark Utilities
char * load_program_source(const char *filename)
{ 
	struct stat statbuf;
	FILE *fh; 
	char *source; 
	
	fh = fopen(filename, "r");
	if (fh == 0){
		return 0; 
  }
	
	stat(filename, &statbuf);
	source = (char *) malloc(statbuf.st_size + 1);
	fread(source, statbuf.st_size, 1, fh);
	source[statbuf.st_size] = '\0'; 

	return source; 
} 

#pragma mark -
#pragma mark Main OpenCL Routine
int runCL(int * a, int * results, int n)
{
	cl_program program[1];
	cl_kernel kernel[2];
	
	cl_command_queue cmd_queue;
	cl_context   context;
	
	cl_device_id cpu = NULL, device = NULL;

	cl_int err = 0;
	size_t returned_size = 0;
	size_t buffer_size;
	
	cl_mem a_mem, ans_mem;
	
#pragma mark Device Information
	{
		// Find the CPU CL device, as a fallback
		err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_CPU, 1, &cpu, NULL);
		assert(err == CL_SUCCESS);
		
		// Find the GPU CL device, this is what we really want
		// If there is no GPU device is CL capable, fall back to CPU
		err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
		if (err != CL_SUCCESS) device = cpu;
		assert(device);
	
		// Get some information about the returned device
		cl_char vendor_name[1024] = {0};
		cl_char device_name[1024] = {0};
		err = clGetDeviceInfo(device, CL_DEVICE_VENDOR, sizeof(vendor_name), 
							  vendor_name, &returned_size);
		err |= clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(device_name), 
							  device_name, &returned_size);
		assert(err == CL_SUCCESS);
	}
	
#pragma mark Context and Command Queue
	{
		// Now create a context to perform our calculation with the 
		// specified device 
		context = clCreateContext(0, 1, &device, NULL, NULL, &err);
		assert(err == CL_SUCCESS);
		
		// And also a command queue for the context
//		cmd_queue = clCreateCommandQueue(context, device, 0, NULL);
		cmd_queue = clCreateCommandQueue(context, device, 0, &err);
		assert(err == CL_SUCCESS);
	}
	
#pragma mark Program and Kernel Creation
	{
		// Load the program source from disk
		// The kernel/program is the project directory and in Xcode the executable
		// is set to launch from that directory hence we use a relative path
		const char * filename = "fibs.cl";
		char *program_source = load_program_source(filename);
		program[0] = clCreateProgramWithSource(context, 1, (const char**)&program_source,
											   NULL, &err);
		assert(err == CL_SUCCESS);
		
		err = clBuildProgram(program[0], 0, NULL, NULL, NULL, NULL);
		assert(err == CL_SUCCESS);
		
		// Now create the kernel "objects" that we want to use in the example file 
		kernel[0] = clCreateKernel(program[0], "fibs", &err);
	}
		
#pragma mark Memory Allocation
	{
		// Allocate memory on the device to hold our data and store the results into
		buffer_size = sizeof(int) * n;
		
		// Input array a
		a_mem = clCreateBuffer(context, CL_MEM_READ_ONLY, buffer_size, NULL, NULL);
		err = clEnqueueWriteBuffer(cmd_queue, a_mem, CL_TRUE, 0, buffer_size,
								   (void*)a, 0, NULL, NULL);
		
		// Results array
		ans_mem	= clCreateBuffer(context, CL_MEM_READ_WRITE, buffer_size, NULL, NULL);
		
		// Get all of the stuff written and allocated 
		clFinish(cmd_queue);
	}
	
#pragma mark Kernel Arguments
	{
		// Now setup the arguments to our kernel
		err  = clSetKernelArg(kernel[0],  0, sizeof(cl_mem), &a_mem);
		err |= clSetKernelArg(kernel[0],  1, sizeof(cl_mem), &ans_mem);
		assert(err == CL_SUCCESS);
	}
	
#pragma mark Execution and Read
	{
		// Run the calculation by enqueuing it and forcing the 
		// command queue to complete the task
		size_t global_work_size = n;
		err = clEnqueueNDRangeKernel(cmd_queue, kernel[0], 1, NULL, 
									 &global_work_size, NULL, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
		clFinish(cmd_queue);
		
		// Once finished read back the results from the answer 
		// array into the results array
		err = clEnqueueReadBuffer(cmd_queue, ans_mem, CL_TRUE, 0, buffer_size, 
								  results, 0, NULL, NULL);
		assert(err == CL_SUCCESS);
		clFinish(cmd_queue);
	}
	
#pragma mark Teardown
	{
		clReleaseMemObject(a_mem);
		clReleaseMemObject(ans_mem);
		
		clReleaseCommandQueue(cmd_queue);
		clReleaseContext(context);
	}
	return CL_SUCCESS;
}

int get_random_number(int maxValue) {
  return rand()%maxValue+1;
}

Handle<Value> ArrToFib(const Arguments& args) {
  HandleScope scope;

  Handle<Array> arrIn = Handle<Array>::Cast(args[0]);
	// Problem size
  int n = arrIn->Length();

 	// Allocate some memory and a place for the results
	int * a = (int *)malloc(n*sizeof(int));
	int * results = (int *)malloc(n*sizeof(int));
	
	// Fill in the values
	for(int i=0;i<n;i++){
		a[i] = (int)arrIn->Get(i).As<v8::Integer>()->Int32Value();
		results[i] = 0;
	}

 	// Do the OpenCL calculation
	runCL(a, results, n);

  // Array to return results
	Local<Array> array = Array::New(n);
 	for(int j=0;j<n;j++) {
    array->Set(j, Integer::New(results[j])); 
  }

	// Free up memory
	free(a);
	free(results);
	
  return  scope.Close(array);
}

void Init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("ArrToFib"),
      FunctionTemplate::New(ArrToFib)->GetFunction());
}

NODE_MODULE(fibs, Init)

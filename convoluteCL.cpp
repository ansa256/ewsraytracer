#include "convoluteCL.h"
#include <cl.h>
#include "Utility/SDL_Utility.h"
#include <string>
#include <fstream>

using namespace std;

string readFile(const char *filename) {
   ifstream in(filename, ios::in | ios::binary);
   if (in) {
      string contents;
      in.seekg(0, std::ios::end);
      contents.resize(in.tellg());
      in.seekg(0, std::ios::beg);
      in.read(&contents[0], contents.size());
      in.close();
      return(contents);
   }
   return "";
}

SDL_Surface* convoluteCL(SDL_Surface* source, float* filter, int fsize) {
   float* cs = new float[source->w * source->h * 4];
   Uint8 r, g, b, a;
   float cf = 1.f / 255.f;
   
   int idx = 0;
   for (int y = 0; y < source->h; y++) {
      for (int x = 0; x < source->w; x++) {
         Uint32 pixel = getPixel(source, x, y);
         SDL_GetRGBA(pixel, source->format, &r, &g, &b, &a);
         cs[idx++] = r * cf;
         cs[idx++] = g * cf;
         cs[idx++] = b * cf;
         cs[idx++] = a * cf;
      }
   }

   cl_platform_id platform;
   cl_int status = clGetPlatformIDs(1, &platform, NULL);
   printf("platform = %d\n", status);
   
   // Device discovery
   cl_uint nDevices;
   status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, NULL, &nDevices);
   printf("Device count = %d\n", status);
   
   cl_device_id* devices = new cl_device_id[nDevices];
   status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, nDevices, devices, NULL);
   printf("Devices = %d\n", status);

   // Create context
   cl_context context = clCreateContext(NULL, nDevices, devices, NULL, NULL, &status);
   printf("Context = %d\n", status);
   
   // Create command queue
   cl_command_queue cmdQueue = clCreateCommandQueue(context, devices[1], 0, &status);
   printf("Queue = %d\n", status);

   // Create image format
   cl_image_format format;
   format.image_channel_order = CL_RGBA;
   format.image_channel_data_type = CL_FLOAT;
   
   // Create device buffers
   cl_mem sourceBuf = clCreateImage2D(context, CL_MEM_READ_ONLY, &format, source->w, source->h, 0, NULL, &status);
   printf("Source buffer = %d\n", status);

   cl_mem destBuf = clCreateImage2D(context, 0, &format, source->w, source->h, 0, NULL, &status);
   printf("Destination buffer = %d\n", status);

   cl_mem filterBuf = clCreateBuffer(context, CL_MEM_READ_ONLY, fsize * fsize * sizeof(float), NULL, &status);
   printf("Filter buffer = %d\n", status);
   
   // Copy data to device buffers
   size_t origin[3] = {0, 0, 0};
   size_t region[3] = {source->w, source->h, 1};
   
   status = clEnqueueWriteImage(cmdQueue, sourceBuf, CL_FALSE, origin, region, 0, 0, cs, 0, NULL, NULL);
   printf("Enqueue image = %d\n", status);

   status = clEnqueueWriteBuffer(cmdQueue, filterBuf, CL_FALSE, 0, fsize * fsize * sizeof(float), filter, 0, NULL, NULL);
   printf("Enqueue filter = %d\n", status);
   
   // Create sampler
   cl_sampler sampler = clCreateSampler(context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST, &status);
   printf("Sampler = %d\n", status);
   
   // Create and compile the program
   string code = readFile("convolution.kernel");
   cl_program program = clCreateProgramWithSource(context, 1, (const char**) &code, NULL, &status);
   printf("Program = %d\n", status);
   
   status = clBuildProgram(program, nDevices, devices, NULL, NULL, NULL);
   printf("Build = %d\n", status);
   
   size_t length;
   clGetProgramBuildInfo(program, devices[1], CL_PROGRAM_BUILD_LOG, 0, NULL, &length);
   char* buffer = new char[length];
   clGetProgramBuildInfo(program, devices[1], CL_PROGRAM_BUILD_LOG, length, buffer, NULL);
//   printf("%s\n", buffer);
   delete[] buffer;

   // Create the kernel
   cl_kernel kernel = clCreateKernel(program, "convolution", &status);
   printf("kernel = %d\n", status);

   // Set the kernel arguments
   status  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &sourceBuf);
   status |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &destBuf);
   status |= clSetKernelArg(kernel, 2, sizeof(cl_int), &source->h);
   status |= clSetKernelArg(kernel, 3, sizeof(cl_int), &source->w);
   status |= clSetKernelArg(kernel, 4, sizeof(cl_mem), &filterBuf);
   status |= clSetKernelArg(kernel, 5, sizeof(cl_int), &fsize);
   status |= clSetKernelArg(kernel, 6, sizeof(cl_sampler), &sampler);
   printf("set arg = %d\n", status);
   
   // Configure the work-item structure
   size_t globalws[2] = {source->w, source->h};
   size_t localws[2] = {10, 10};
   
   // Enqueue the kernel for execution
   status = clEnqueueNDRangeKernel(cmdQueue, kernel, 2, NULL, globalws, localws, 0, NULL, NULL);
   printf("NDRange = %d\n", status);

   // Read the output buffer back to the host
   float* cd = new float[source->w * source->h * 4];
   status = clEnqueueReadImage(cmdQueue, destBuf, CL_TRUE, origin, region, 0, 0, cd, 0, NULL, NULL);
   printf("ReadBuffer = %d\n", status);

   SDL_Surface* dest = createSurface(source->w, source->h);
   idx = 0;
   for (int y = 0; y < source->h; y++) {
      for (int x = 0; x < source->w; x++) {
         Uint32 pixel = SDL_MapRGBA(source->format, cd[idx++] * 255, cd[idx++] * 255, cd[idx++] * 255, cd[idx++] * 255);
         setPixel(dest, x, y, pixel);
      }
   }
   
   // Release OpenCL resources
   clReleaseKernel(kernel);
   clReleaseProgram(program);
   clReleaseCommandQueue(cmdQueue);
   clReleaseMemObject(sourceBuf);
   clReleaseMemObject(destBuf);
   clReleaseMemObject(filterBuf);
   clReleaseSampler(sampler);
   clReleaseContext(context);
   
   delete[] cs;
   delete[] cd;
   delete[] devices;
   
   return dest;
}

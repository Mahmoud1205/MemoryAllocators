/// @file Example usage.

#include "MemoryAllocators.h"
#include <cstdio>

struct ExampleStruct
{
	unsigned int mCount;
	float mFloat;
};

// example error callback
static void errorCallback(const char* inErrorMessage)
{
	fprintf(stderr, inErrorMessage);
	// c++ exception handling can be added here
}

int main(void)
{
	Mem::errorCallback = &errorCallback;

	{ // Bump Allocator Example
		printf("Bump Allocator Example:\n");

		Mem::BumpAllocator bumpAllocator{};
		bumpAllocator.Create(Mem::SizeKB(1));

		int* mem1 = (int*)bumpAllocator.Alloc(sizeof(int));
		*mem1 = 7;

		float* mem2 = (float*)bumpAllocator.Alloc(sizeof(float));
		*mem2 = 3.14;

		ExampleStruct* mem3 = (ExampleStruct*)bumpAllocator.Alloc(sizeof(ExampleStruct));
		*mem3 = ExampleStruct{7, 1.0f};

		printf("mem1 = %i\n", *mem1);
		printf("mem2 = %f\n", *mem2);
		printf("mem3->mCount = %u\n", mem3->mCount);
		printf("mem3->mFloat = %f\n", mem3->mFloat);
		printf("%zu bytes left.\n", bumpAllocator.GetRemainingBytes());

		bumpAllocator.Reset();
		bumpAllocator.Destroy();
		putchar('\n');
	}

	{ // Pool Allocator Example
		printf("Pool Allocator Example:\n");

		Mem::PoolAllocator poolAllocator{};
		poolAllocator.Create(sizeof(double), 8);
		poolAllocator.PrintUsage();

		Mem::PoolAllocator::Allocation mem1 = poolAllocator.Alloc();
		*(double*)mem1.mMemory = 7.0;
		printf("mem1 = %f\n", *(double*)mem1.mMemory);
		poolAllocator.PrintUsage();

		poolAllocator.Free(mem1);
		printf("mem1 freed!\n");
		poolAllocator.PrintUsage();

		Mem::PoolAllocator::Allocation mem2 = poolAllocator.Alloc();
		*(double*)mem2.mMemory = 8.0;
		printf("mem2 = %f\n", *(double*)mem2.mMemory);

		Mem::PoolAllocator::Allocation mem3 = poolAllocator.Alloc();
		*(double*)mem3.mMemory = 9.0;
		printf("mem3 = %f\n", *(double*)mem3.mMemory);
		poolAllocator.PrintUsage();

		poolAllocator.Reset();
		printf("all memory freed!\n");
		poolAllocator.PrintUsage();

		for (size_t i = 0; i < poolAllocator.GetNumMaxElements(); i++)
		{
			Mem::PoolAllocator::Allocation mem0 = poolAllocator.Alloc();
		}
		printf("memory filled!\n");
		poolAllocator.PrintUsage();

		poolAllocator.Reset();
		poolAllocator.Destroy();
		putchar('\n');
	}
}

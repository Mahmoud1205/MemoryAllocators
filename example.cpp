/// @file Example usage.

#include "MemoryAllocators.h"
#include <cstdio>

struct ExampleStruct
{
	unsigned int mCount;
	float mFloat;
};

using namespace Mem;

int main(void)
{
	{ // Stack Allocator Example
		printf("Stack Allocator Example:\n");

		StackAllocator stackAllocator{};
		stackAllocator.Create(SizeKB(1));

		int* mem1 = (int*)stackAllocator.Alloc(sizeof(int));
		*mem1 = 7;

		float* mem2 = (float*)stackAllocator.Alloc(sizeof(float));
		*mem2 = 3.14;

		ExampleStruct* mem3 = (ExampleStruct*)stackAllocator.Alloc(sizeof(ExampleStruct));
		*mem3 = ExampleStruct{7, 1.0f};

		printf("mem1 = %i\n", *mem1);
		printf("mem2 = %f\n", *mem2);
		printf("mem3->mCount = %u\n", mem3->mCount);
		printf("mem3->mFloat = %f\n", mem3->mFloat);
		printf("%zu bytes left.\n", stackAllocator.GetRemainingBytes());

		stackAllocator.Reset();
		stackAllocator.Destroy();
		putchar('\n');
	}

	{ // Pool Allocator Example
		printf("Pool Allocator Example:\n");

		PoolAllocator poolAllocator{};
		poolAllocator.Create(sizeof(double), 8);
		poolAllocator.PrintUsage();

		PoolAllocator::Allocation mem1 = poolAllocator.Alloc();
		*(int*)mem1.mMemory = 7;
		printf("mem1 = %i\n", *(int*)mem1.mMemory);
		poolAllocator.PrintUsage();

		poolAllocator.Free(mem1);
		printf("mem1 freed!\n");
		poolAllocator.PrintUsage();

		PoolAllocator::Allocation mem2 = poolAllocator.Alloc();
		*(int*)mem2.mMemory = 8;
		printf("mem2 = %i\n", *(int*)mem2.mMemory);

		PoolAllocator::Allocation mem3 = poolAllocator.Alloc();
		*(int*)mem3.mMemory = 9;
		printf("mem3 = %i\n", *(int*)mem3.mMemory);
		poolAllocator.PrintUsage();

		poolAllocator.Reset();
		printf("all memory freed!\n");
		poolAllocator.PrintUsage();

		for (size_t i = 0; i < poolAllocator.GetNumMaxElements(); i++)
		{
			PoolAllocator::Allocation mem0 = poolAllocator.Alloc();
		}
		printf("memory filled!\n");
		poolAllocator.PrintUsage();

		poolAllocator.Reset();
		poolAllocator.Destroy();
		putchar('\n');
	}
}

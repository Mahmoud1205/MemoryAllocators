/// @file Example usage.

#include "MemoryAllocators.h"
#include <cstdio>

struct ExampleStruct
{
	unsigned int mCount;
	float mFloat;
};

using namespace Mem;

i32 main(void)
{
	{ // Stack Allocator Example
		printf("Stack Allocator Example:\n");

		StackAllocator stackAllocator{};
		stackAllocator.Create(SizeKB(1));

		i32* mem1 = (i32*)stackAllocator.Alloc(sizeof(i32));
		*mem1 = 7;

		f32* mem2 = (f32*)stackAllocator.Alloc(sizeof(f32));
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
		poolAllocator.Create(sizeof(f64), 8);
		poolAllocator.PrintUsage();

		PoolAllocator::Allocation mem1 = poolAllocator.Alloc();
		*(i32*)mem1.mMemory = 7;
		printf("mem1 = %i\n", *(i32*)mem1.mMemory);
		poolAllocator.PrintUsage();

		poolAllocator.Free(mem1);
		printf("mem1 freed!\n");
		poolAllocator.PrintUsage();

		PoolAllocator::Allocation mem2 = poolAllocator.Alloc();
		*(i32*)mem2.mMemory = 8;
		printf("mem2 = %i\n", *(i32*)mem2.mMemory);

		PoolAllocator::Allocation mem3 = poolAllocator.Alloc();
		*(i32*)mem3.mMemory = 9;
		printf("mem3 = %i\n", *(i32*)mem3.mMemory);
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

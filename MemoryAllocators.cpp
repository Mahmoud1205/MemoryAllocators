#include "MemoryAllocators.h"

#include <malloc.h>
#include <string.h>
#include <cstdio>

void (*Mem::errorCallback)(const char* inErrorMessage) = nullptr;

using namespace Mem;

void StackAllocator::Create(size_t inMaxSize)
{
	mMaxSize = inMaxSize;
	mMemory = (char*)malloc(mMaxSize);
	Reset();
}

void StackAllocator::Destroy()
{
	free(mMemory);
}

void* StackAllocator::Alloc(size_t inSize)
{
	if (mStackPtr + inSize > mMaxSize)
	{
		if (errorCallback != nullptr)
		{
			char errorMessage[512];
			snprintf(errorMessage, sizeof(errorMessage), "Stack Allocator Error: Failed to allocate memory, the size of the requested allocation (%zu) is bigger than the remaining size left in the allocator (%zu).\n", inSize, mMaxSize - mStackPtr);
			errorCallback(errorMessage);
		}
		return nullptr;
	}

	void* requestedMemory = mMemory + mStackPtr;
	mStackPtr += inSize;
	return requestedMemory;
}

void StackAllocator::Reset()
{
	mStackPtr = 0;
}

void PoolAllocator::Create(size_t inElementSize, size_t inMaxElements)
{
	mElementSize = inElementSize;
	mMaxElements = inMaxElements;
	mCellUsage = (bool*)malloc(mMaxElements);
	mMemory = (char*)malloc(mElementSize * mMaxElements);
	Reset();
}

void PoolAllocator::Destroy()
{
	free(mMemory);
}

PoolAllocator::Allocation PoolAllocator::Alloc()
{
	for (size_t i = 0; i < mMaxElements; i++)
	{
		if (mCellUsage[i] == false) 
		{
			mCellUsage[i] = true;
			return Allocation{ mMemory + (mElementSize * i), i };
		}
	}

	if (errorCallback != nullptr)
	{
		char errorMessage[256];
		snprintf(errorMessage, sizeof(errorMessage), "Pool Allocator Error: Failed to allocate memory, there are no more free cells in the memory pool.\n");
		errorCallback(errorMessage);
	}
	// mCellIdx is set to mMaxElements + 1 so the user doesn't accidentally free memory used by another if he doesnt acknowledge that the allocation failed.. this is checked by Free()
	return Allocation{ nullptr, mMaxElements + 1 };
}

void PoolAllocator::Free(const Allocation& inAllocation)
{
	if (inAllocation.mCellIdx >= mMaxElements || inAllocation.mMemory == nullptr)
	{ // the user is trying to free memory that does not exist..
		if (errorCallback != nullptr)
		{
			char errorMessage[256];
			snprintf(errorMessage, sizeof(errorMessage), "Pool Allocator Error: Failed to free memory, attempted to free memory that does not exist.\n");
			errorCallback(errorMessage);
		}
		return;
	}
	mCellUsage[inAllocation.mCellIdx] = false;
}

void PoolAllocator::Reset()
{
	memset(mCellUsage, false, mMaxElements);
}

size_t PoolAllocator::GetNumFreeCells() const
{
	size_t numRemainingElements = 0;
	for (size_t i = 0; i < mMaxElements; i++)
		if (mCellUsage[i] == 0)
			numRemainingElements++;

	return numRemainingElements;
}

bool PoolAllocator::IsFull() const
{
	for (size_t i = 0; i < mMaxElements; i++)
		if (mCellUsage[i] == 0)
			return false;

	return true;
}

void PoolAllocator::PrintUsage() const
{
	printf("Memory Pool Usage: [");
	for (size_t i = 0; i < mMaxElements; i++)
		printf("%s", mCellUsage[i] == true ? "#" : ".");
	printf("]\n");
}

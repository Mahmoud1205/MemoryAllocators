#include "MemoryAllocators.h"

#include <malloc.h>
#include <string.h>
#include <cstdio>

void (*Mem::errorCallback)(const char* inErrorMessage) = nullptr;

using namespace Mem;

void BumpAllocator::Create(size_t inMaxSize)
{
	mMaxSize = inMaxSize;
	mMemory = (char*)malloc(mMaxSize);
	Reset();
}

void BumpAllocator::Destroy()
{
	free(mMemory);
}

void* BumpAllocator::Alloc(size_t inSize)
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

void BumpAllocator::Reset()
{
	mStackPtr = 0;
}

void PoolAllocator::Create(size_t inElementSize, size_t inMaxElements)
{
	mElementSize = inElementSize;
	mMaxElements = inMaxElements;
	mAllocations = (allocation_t*)malloc(mMaxElements * sizeof(*mAllocations));
	for (size_t i = 0; i < mMaxElements; i++)
		mAllocations[i].mMemory = malloc(mElementSize);
	Reset();
}

void PoolAllocator::Destroy()
{
	free(mAllocations);
}

void* PoolAllocator::Alloc()
{
	for (size_t i = 0; i < mMaxElements; i++)
	{
		if (!mAllocations[i].mIsUsed)
		{
			mAllocations[i].mIsUsed = true;
			return mAllocations[i].mMemory;
		}
	}

	if (errorCallback != nullptr)
	{
		char errorMessage[256];
		snprintf(errorMessage, sizeof(errorMessage), "Pool Allocator Error: Failed to allocate memory, there are no more free cells in the memory pool.\n");
		errorCallback(errorMessage);
	}
	return nullptr;
}

void PoolAllocator::Free(void* inMemory)
{
	for (size_t i = 0; i < mMaxElements; i++)
		if (mAllocations[i].mMemory == inMemory)
			mAllocations[i].mIsUsed = false;
}

void PoolAllocator::Reset()
{
	for (size_t i = 0; i < mMaxElements; i++)
		mAllocations[i].mIsUsed = false;
}

size_t PoolAllocator::GetNumFreeCells() const
{
	size_t numRemainingElements = 0;
	for (size_t i = 0; i < mMaxElements; i++)
		if (mAllocations[i].mIsUsed == 0)
			numRemainingElements++;

	return numRemainingElements;
}

bool PoolAllocator::IsFull() const
{
	for (size_t i = 0; i < mMaxElements; i++)
		if (mAllocations[i].mIsUsed == 0)
			return false;

	return true;
}

void PoolAllocator::PrintUsage() const
{
	printf("Memory Pool Usage: [");
	for (size_t i = 0; i < mMaxElements; i++)
		printf("%s", mAllocations[i].mIsUsed == true ? "#" : ".");
	printf("]\n");
}

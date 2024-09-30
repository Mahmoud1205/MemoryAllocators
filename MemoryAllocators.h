#pragma once

namespace Mem
{
	/// @brief Error callback called by allocators when an error happens. You can add exception handling to this or connect it to your logger.
	/// @param inErrorMessage A descriptive error message.
	extern void (*errorCallback)(const char* inErrorMessage);

	constexpr size_t SizeKB(size_t inKB) { return inKB * 1024; }
	constexpr size_t SizeMB(size_t inMB) { return inMB * 1024 * 1024; }
	constexpr size_t SizeGB(size_t inGB) { return inGB * 1024 * 1024 * 1024; }

	/// @brief Very simple stack allocator.
	class BumpAllocator final
	{
	public:
				BumpAllocator() = default;
				~BumpAllocator() = default;

		/// @brief Creates the allocator to be used.
		/// @param inMaxSize The maximum size in bytes to be used by the allocator. Use the Mem::SizeKB/MB/GB() functions to get bytes from other units.
		void	Create(size_t inMaxSize);
		/// @brief Destroys the allocator and frees all of it's memory.
		void	Destroy();
		/// @brief Requests a block of memory to be allocated.
		/// @param inSize Size of the block in bytes. Use the Mem::SizeKB/MB/GB() functions to get bytes from other units.
		/// @return Pointer to the block.
		void*	Alloc(size_t inSize);
		/// @brief Resets the memory, this does not clear it, only frees all the space for future allocations.
		void	Reset();

		/// @brief Get the maximum size for the allocator which was set by the user in Create().
		size_t	GetMaxSize() const { return mMaxSize; }
		/// @brief Get the remaining memory in bytes.
		size_t	GetRemainingBytes() const { return mMaxSize - mStackPtr; }
		/// @brief Returns true if there is no more memory at all.
		bool	IsFull() const { return mStackPtr >= mMaxSize; }

	private:
		char*	mMemory;
		size_t	mMaxSize;
		size_t	mStackPtr;
	};

	/// @brief Pool allocator.
	class PoolAllocator final
	{
	public:
					PoolAllocator() = default;
					~PoolAllocator() = default;

		/// @brief Creates the allocator to be used.
		/// @param inElementSize The size of each element that will be allocated.
		/// @param inMaxElements The number of elements to be allocated.
		void		Create(size_t inElementSize, size_t inMaxElements);
		/// @brief Destroys the allocator and frees all of it's memory.
		void		Destroy();
		/// @brief Requests a block of memory to be allocated.
		/// @return Pointer to the block.
		void*		Alloc();
		/// @brief Frees an allocation.
		/// @param inMemory The block of memory to be freed.
		void		Free(void* inMemory);
		/// @brief Frees all allocations.
		void		Reset();

		/// @brief Get the size of each element.
		size_t		GetElementSize() const { return mElementSize; }
		/// @brief Get the maximum number of elements.
		size_t		GetNumMaxElements() const { return mMaxElements; }
		/// @brief Get the number of free cells.
		size_t		GetNumFreeCells() const;
		/// @brief Returns true if there is no more memory at all.
		bool		IsFull() const;
		/// @brief Prints the usage of the pool allocator. '#' is used and '.' is unused.
		void		PrintUsage() const;

	private:
		struct allocation_t {
			void* mMemory;
			bool mIsUsed;
			char mPadding[7];
		} *mAllocations;

		size_t		mElementSize;
		size_t		mMaxElements;
	};
}

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

		/// @brief A memory allocation gotten from Alloc(), do not make this directly.
		struct Allocation
		{
			/// @brief The memory block you requested.
			void*	mMemory;
			/// @brief The index of the cell you were given, do not edit this, it is used by Free().
			size_t	mCellIdx;
		};

		/// @brief Creates the allocator to be used.
		/// @param inElementSize The size of each element that will be allocated.
		/// @param inMaxElements The number of elements to be allocated.
		void		Create(size_t inElementSize, size_t inMaxElements);
		/// @brief Destroys the allocator and frees all of it's memory.
		void		Destroy();
		/// @brief Requests a block of memory to be allocated.
		/// @return See documentation for PoolAllocator::Allocation.
		Allocation	Alloc();
		/// @brief Frees an allocation.
		/// @param inAllocation The allocation to be freed.
		void		Free(const Allocation& inAllocation);
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
		char*		mMemory;
		bool*		mCellUsage; // TODO: change to bitset
		size_t		mElementSize;
		size_t		mMaxElements;
	};
}

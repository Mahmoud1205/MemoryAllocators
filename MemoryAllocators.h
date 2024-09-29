#pragma once

#define MA_STATIC_ASSERT static_assert

namespace Mem
{
#pragma region Fixed-length types
	using i8 = signed char;
	MA_STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");

	using u8 = unsigned char;
	MA_STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");

	using u16 = unsigned short int;
	MA_STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");

	using i16 = short int;
	MA_STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");

	using i32 = int;
	MA_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");

	using u32 = unsigned int;
	MA_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");

	using i64 = long long int;
	MA_STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

	using u64 = unsigned long long int;
	MA_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

	using f32 = float;
	MA_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

	using f64 = double;
	MA_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");
#pragma endregion

	constexpr size_t SizeKB(size_t inKB) { return inKB * 1024; }
	constexpr size_t SizeMB(size_t inMB) { return inMB * 1024 * 1024; }
	constexpr size_t SizeGB(size_t inGB) { return inGB * 1024 * 1024 * 1024; }

	/// @brief Very simple stack allocator.
	class StackAllocator final
	{
	public:
				StackAllocator() = default;
				~StackAllocator() = default;

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
	class PoolAllocator
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

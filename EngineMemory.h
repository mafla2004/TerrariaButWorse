#ifndef __ENGINEMEMORY__
#define __ENGINEMEMORY__

#define USE_CUSTOM_GARBAGE_COLLECTION 0

namespace Engine
{
	/*	I planned to make a custom garbage collector, but since this is a learning project I decided
	to use C++'s smart pointers. I still plan to implement a custom garbage collector
	at some point though, so I use the preprocessor to "save" what was done as of now... */
#if !USE_CUSTOM_GARBAGE_COLLECTION
	template <class C>
	using Pointer = std::shared_ptr<C>;

	template<class C>
	using Reference = std::weak_ptr<C>;

	template<class C>
	using Ownership = std::unique_ptr<C>;

	template<class C>
	inline Pointer<C> MakePointer() { return std::make_shared<C>(); };

	template<class C>
	inline Pointer<C> MakePointer(size_t n) { return std::make_shared<C>(n); };

	template<class C, class... Args>
	inline Pointer<C> MakePointer(Args&&... args) { return std::make_shared(args); };

	template<class C>
	inline Reference<C> MakeReference(const Pointer<C>& ptr) { return ptr; }

	template<class C>
	inline Reference<C> MakeReference(const Ownership<C>& own) { return own; }

	template<class C>
	inline Ownership<C> MakeOwnership() { return std::make_unique(); }

	template<class C>
	inline Ownership<C> MakeOwnership(size_t n) { return std::make_unique(n); }

	template<class C, class... Args>
	inline Ownership<C> MakeOwnership(Args&&... args) { return std::make_unique(args); }
#else
	template <class C>
	class Pointer;

	template <class C>
	class Reference;

	template <class C>
	class Ownership;

	template<class C>
	Pointer<C> MakePointer();

	template<class C>
	Pointer<C> MakePointer(size_t n);

	template<class C, class... Args>
	Pointer<C> MakePointer(Args&&... args);

	// --------------------------------------------------
	// CUSTOM GARBAGE COLLECTION
	// --------------------------------------------------

	std::unordered_map<Object*, uint32_t> pointers;

	template<class C>
	class Pointer
	{
	private:
		C* raw;

		Pointer();
	public:
		virtual ~Pointer() = default;

		C* operator->() const noexcept;
	};

	template<class C>
	inline C* Pointer<C>::operator->() const noexcept { return raw; }
#endif
}

#endif
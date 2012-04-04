#pragma once

#ifndef RawAlloc_h__
#define RawAlloc_h__

template<class T>
class RawAlloc
{
public:
	// type definitions
	typedef T        value_type;
	typedef T*       pointer;
	typedef const T* const_pointer;
	typedef T&       reference;
	typedef const T& const_reference;
	typedef std::size_t    size_type;
	typedef std::ptrdiff_t difference_type;

	// rebind allocator to type U
	template <class U>
	struct rebind {
		typedef RawAlloc<U> other;
	};

	const T* _addressStart;
	const T* _addressEnd;
	T* _current;

	RawAlloc(const T*  addressStart, const T*  addressEnd)
	{
		_addressStart = addressStart;
		_current = const_cast<T*>(_addressStart);
		_addressEnd = addressEnd;
	}

	std::size_t max_size () const throw() {
		return (_addressEnd - _addressStart) / sizeof(T);
	}

	T* allocate (std::size_t num, const void* = 0) {
		pointer tmp = _current;
		_current += sizeof(T);

		return tmp;
	}

	void deallocate (T* p, std::size_t num) {
		// do nothing
	}
};
#endif // RawAlloc_h__
#pragma once


namespace types
{
	template<class T>
	class simple_ptr
	{
	public:
		simple_ptr()
			: _data(NULL)
		{				
		}

		explicit simple_ptr(T* ptr)
			: _data(ptr)
		{
		}

		simple_ptr(const simple_ptr<T> & right)
			: _data(right._data)
		{

		}

		const simple_ptr& operator=(const simple_ptr<T> & right)
		{
			_data = right._data;
			return *this;
		}

		const simple_ptr& operator=(T* right)
		{
			_data = right;
			return *this;
		}

		operator T*() 
		{
			return _data;
		}

		T* operator->()
		{
			return _data;
		}

		bool operator>(const simple_ptr<T> & right) const
		{
			return this->_data > right._data;
		}

		bool operator<(const simple_ptr<T> & right) const
		{
			return this->_data < right._data;
		}

		bool operator==(const simple_ptr<T> & right) const
		{
			return this->_data == right._data;
		}

		bool IsNull()
		{
			return _data == NULL;
		}

		const simple_ptr<T>& operator+=(const unsigned long & offset)
		{
			_data += offset;
			return *this;
		}

		const simple_ptr<T>& operator-=(const unsigned long & offset)
		{
			_data -= offset;
			return *this;
		}

	protected:
		T* _data;
	};

}
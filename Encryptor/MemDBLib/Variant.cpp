#include "StdAfx.h"
#include "Variant.h"

#include "../Utils/from_string.hpp"
#include "InvalidTypeCastException.h"

#include <iomanip>

#include <boost/foreach.hpp>

namespace types
{
	Variant::Variant()
	{
		//throw types::Exceptions::VariantException("Do not call default variant consructor!");
	}

	Variant::Variant(const __int64 & val)
		: _val(val)
	{
	}
	Variant::Variant(const float & val)
		: _val(val)
	{
	}
	Variant::Variant(const double & val)
		: _val(val)
	{
	}
	Variant::Variant(const int & val)
		: _val(val)
	{
	}
	Variant::Variant(const unsigned int & val)
		: _val(val)
	{
	}
	Variant::Variant(const unsigned char & val)
		: _val(val)
	{
	}
	Variant::Variant(const short & val)
		: _val(val)
	{
	}
	Variant::Variant(const unsigned short & val)
		: _val(val)
	{
	}
	Variant::Variant(const unsigned long & val)
		: _val(val)
	{
	}
	Variant::Variant(const std::string & val)
		: _val(val)
	{
	}

	Variant::~Variant(void)
	{
	}

	Variant::Variant(const Variant & right)
	{
		_val = right._val;
	}

	Variant::Variant(const std::wstring & val)
		: _val(val)
	{		
	}

	Variant::Variant(const std::vector<unsigned char> & val)
		: _val(val)
	{
	}

	Variant::Variant(const GUID & val)
		: _val(val)
	{
	}

	std::string Variant::ToString() const
	{
		std::string rValue = boost::apply_visitor(Variant::ToString_Visitor(), this->_val);
		return rValue;
	}	

	bool Variant::operator<(const Variant& right) const
	{
		if (!boost::apply_visitor(Variant::IsString_Visitor(), this->_val) 
			&& !boost::apply_visitor(Variant::IsString_Visitor(), right._val))
		{
			if (boost::apply_visitor(Variant::IsFractional_Visitor(), this->_val)
				|| (boost::apply_visitor(Variant::IsFractional_Visitor(), right._val)))
			{
				return boost::apply_visitor(Variant::ToDouble_Visitor(), this->_val) 
					 < boost::apply_visitor(Variant::ToDouble_Visitor(), right._val);
			}

			return boost::apply_visitor(Variant::ToInt64_Visitor(), this->_val) 
				 < boost::apply_visitor(Variant::ToInt64_Visitor(), right._val);
		}
		return this->ToString() < right.ToString();
	}

	bool Variant::operator==(const Variant& right) const
	{
		if (!boost::apply_visitor(Variant::IsString_Visitor(), this->_val) 
			&& !boost::apply_visitor(Variant::IsString_Visitor(), right._val))
		{
			if (boost::apply_visitor(Variant::IsFractional_Visitor(), this->_val)
				|| (boost::apply_visitor(Variant::IsFractional_Visitor(), right._val)))
			{
				return boost::apply_visitor(Variant::ToDouble_Visitor(), this->_val) 
					== boost::apply_visitor(Variant::ToDouble_Visitor(), right._val);
			}

			return boost::apply_visitor(Variant::ToInt64_Visitor(), this->_val) 
				== boost::apply_visitor(Variant::ToInt64_Visitor(), right._val);
		}
		return this->ToString() == right.ToString();
	}	
	
	bool Variant::operator<=(const Variant& right) const
	{
		bool less = Variant::operator<(right);
		bool equal = Variant::operator==(right);

		return (less || equal);
	}

	bool Variant::operator>(const Variant& right) const
	{
		return !(Variant::operator<=(right));
	}

	bool Variant::operator>=(const Variant& right) const
	{
		return !(Variant::operator<(right));
	}

	bool Variant::operator!=(const Variant& right) const
	{
		return !(Variant::operator==(right));
	}

	template<class T> std::string Variant::ToString_Visitor::operator() (T val) const		
	{
		std::ostringstream os;
		os << val;
		return os.str();
	}	

	template<> std::string Variant::ToString_Visitor::operator() (std::vector<unsigned char> val) const		
	{
		std::ostringstream os;

		int i=0;
		BOOST_FOREACH(unsigned char item, val)
		{			
			os  << (i == 0 ? "" : " ") 
				<< std::hex 
				<< std::setw(2) 
				<< std::setfill('0') 
				<< (unsigned int)item;
			i++;
		}
		
		return os.str();
	}	 

	template<class T> std::vector<unsigned char>  Variant::ToVector_Visitor::operator() (T val) const
	{
		throw memdb::Exceptions::InvalidTypeCastException("Cannot convert to std::vector<unsigned char>");
	}

	template<> std::vector<unsigned char> Variant::ToVector_Visitor::operator() (std::vector<unsigned char> val) const
	{
		return val;
	}


	template<class T> bool Variant::IsString_Visitor::operator() (T val) const
	{
		return false;
	}

	template<> bool Variant::IsString_Visitor::operator() (std::string val) const
	{
		return true;
	}

	template<class T> bool Variant::IsFractional_Visitor::operator() (T val) const
	{
		return false;
	}

	template<> bool Variant::IsFractional_Visitor::operator ()(float val) const
	{
		return true;
	}

	template<> bool Variant::IsFractional_Visitor::operator ()(double val) const
	{
		return true;
	}

	template<class T> double Variant::ToDouble_Visitor::operator ()(T val) const 
	{
#pragma warning (push)
#pragma warning (disable: 4244)
		return (double)val;
#pragma warning (pop)
	}	

	template<> double Variant::ToDouble_Visitor::operator ()(std::string val) const 
	{
		double rValue = 0;
		types::from_string(rValue, val, std::dec);

		return rValue;
	}

	template<> double Variant::ToDouble_Visitor::operator ()(std::wstring val) const 
	{
		throw memdb::Exceptions::InvalidTypeCastException("Cannot convert std::wstring to double");
	}

	template<> double Variant::ToDouble_Visitor::operator ()(std::vector<unsigned char> val) const 
	{
		throw memdb::Exceptions::InvalidTypeCastException("Cannot convert std::vector<unsigned char> to double");
	}

	template<> double Variant::ToDouble_Visitor::operator ()(GUID val) const 
	{
		throw memdb::Exceptions::InvalidTypeCastException("Cannot convert GUID to double");
	}

	template<class T> __int64 Variant::ToInt64_Visitor::operator ()(T val) const 
	{
#pragma warning (push)
#pragma warning (disable: 4244)
		return (__int64) val;
#pragma warning (pop)
	}

	template<> __int64 Variant::ToInt64_Visitor::operator () (std::string val) const 
	{
		__int64 rValue = 0;
		types::from_string(rValue, val, std::dec);

		return rValue;
	}

	template<> __int64 Variant::ToInt64_Visitor::operator ()(std::wstring val) const 
	{
		throw memdb::Exceptions::InvalidTypeCastException("Cannot convert std::wstring to double");
	}

	template<> __int64 Variant::ToInt64_Visitor::operator ()(std::vector<unsigned char> val) const 
	{
		throw memdb::Exceptions::InvalidTypeCastException("Cannot convert std::vector<unsigned char> to double");
	}

	template<> __int64 Variant::ToInt64_Visitor::operator ()(GUID val) const 
	{
		throw memdb::Exceptions::InvalidTypeCastException("Cannot convert GUID to double");
	}

	Variant::operator __int64()
	{
		return boost::apply_visitor(Variant::ToInt64_Visitor(), this->_val);
	}

	Variant::operator std::string()
	{
		return this->ToString();
	}

	Variant::operator std::vector<unsigned char>()
	{
		return boost::apply_visitor(Variant::ToVector_Visitor(), this->_val);
	}
}

std::ostream& operator<<(std::ostream& os, const types::Variant& c)
{
	os << c.ToString();
	return os;
}

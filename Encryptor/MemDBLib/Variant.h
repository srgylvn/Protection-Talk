#pragma once

#include <wtypes.h>

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <boost/variant.hpp>

namespace types
{
	namespace Exceptions
	{
		class VariantException
			: public std::domain_error
		{
		public:
			VariantException(const std::string & text)
				: std::domain_error(text)
			{

			}
			~VariantException(void)
			{
			}
		};
	}

	class Variant
	{	
	public:
		typedef boost::variant<__int64, float, double, int, unsigned int, unsigned char, short, unsigned short, unsigned long, std::string, std::wstring, std::vector<unsigned char>, GUID> TVarType;	
		
		Variant();

		Variant(const __int64 & val);
		Variant(const float & val);
		Variant(const double & val);
		Variant(const int & val);
		Variant(const unsigned int & val);
		Variant(const unsigned char & val);
		Variant(const short & val);
		Variant(const unsigned short & val);
		Variant(const unsigned long & val);
		Variant(const std::string & val);
		Variant(const std::wstring & right);
		Variant(const std::vector<unsigned char> & right);
		Variant(const GUID & right);

		Variant(const Variant & right);
		
		~Variant(void);

		std::string ToString() const;

		bool operator<(const Variant& right) const;
		bool operator==(const Variant& right) const;		
		bool operator>(const Variant& right) const;
		bool operator<=(const Variant& right) const;
		bool operator>=(const Variant& right) const;
		bool operator!=(const Variant& right) const;

		operator __int64();
		operator std::string();
		operator std::vector<unsigned char>();

	protected:
		TVarType _val;
 
	protected:
		class ToVector_Visitor
			: public boost::static_visitor<std::vector<unsigned char>>
		{
		public:
			template<class T> std::vector<unsigned char> operator() (T val) const;
		};

		class ToString_Visitor
			: public boost::static_visitor<std::string>
		{
		public:
			template<class T> std::string operator() (T val) const;			
		};

		class IsString_Visitor
			: public boost::static_visitor<bool>
		{
		public:			
			template<class T> bool operator() (T val) const;			
		};

		
		class IsFractional_Visitor
			: public boost::static_visitor<bool>
		{
		public:
			template<class T> bool operator() (T val) const;	
		};

		class ToDouble_Visitor
			: public boost::static_visitor<double>
		{
		public:
			template<class T> double operator() (T val) const;	
		};

		class ToInt64_Visitor
			: public boost::static_visitor<__int64>
		{
		public:
			template<class T> __int64 operator() (T val) const;	
		};
	};
}

std::ostream& operator<<(std::ostream& os, const types::Variant& c);
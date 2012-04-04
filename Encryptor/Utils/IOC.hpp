#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/concept_check.hpp>

namespace ioc
{
	class IocFactory
	{
	protected:
		template <class T> static boost::shared_ptr<T> NotImplemented();
	public:
		template <class T> static boost::shared_ptr<T> Create()
		{
			return NotImplemented<T>();
		}
	};
	
	#define DECLARE_CONCRETE(TInterface, TConcreteClass) \
	template<> \
	static boost::shared_ptr<TInterface> \
	IocFactory::Create<TInterface>() \
	{ \
		BOOST_CONCEPT_ASSERT((boost::Convertible<TConcreteClass*, TInterface*>)); \
		return boost::shared_ptr<TInterface>(new TConcreteClass()); \
	}
}
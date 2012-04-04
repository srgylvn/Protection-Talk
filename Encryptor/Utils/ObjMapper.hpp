#pragma once

namespace utils
{
	template<class TDestination>
	class ObjMapper
	{
	public:
		ObjMapper(TDestination & destination)
			: _destination(destination) { }
		template <class TSource> ObjMapper<TDestination>& Map(TSource & source) {
			return NotImplemented(source);
		}
		
		TDestination & Destination() {
			return _destination;
		}

	protected:
		template <class TSource> ObjMapper<TDestination>& NotImplemented(TSource & source);
		
	private: 
		TDestination & _destination;
	};
}

#define MAPPER_START(TDest, TSource) \
	template<> \
	template<> \
	utils::ObjMapper<TDest>& utils::ObjMapper<TDest>::Map<TSource>(TSource & source) \
	{

#define MAPPER_END() \
	return *this; \
	}
	
#define MAP_COPY(to, from) \
{ \
	Destination().to = source.from; \
}

#define MAP_CALL_VALUE(setFunction, from) \
{ \
	Destination().setFunction(source.from); \
}

#define MAP_CALL(setFunction, getFunction) \
{ \
	Destination().setFunction(source.getFunction()); \
}
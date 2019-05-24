#ifndef SMPS_STL_COLLECTIONS_SERIALIZER_H_
#define SMPS_STL_COLLECTIONS_SERIALIZER_H_
#include "serializer.h"
namespace smps {

	namespace stl
	{

		template<class T1, class T2>
		class PairWrapper :public Serializable
		{
			T1 first;
			T2 second;

		public:

			PairWrapper(const std::pair<T1, T2>& pair) : first(pair.first), second(pair.second) {}

			SMPS_FIELDS(2)
				SMPS_FIELD(1, first)
				SMPS_FIELD(2, second)

		};

		struct Mapper
		{
			template<class ObjectType>
			static const ObjectType& Map(const ObjectType& obj)
			{
				return obj;
			}

			template<class T1, class T2>
			static const PairWrapper<T1, T2> Map(const std::pair<T1, T2>& obj)
			{
				return PairWrapper<T1, T2>(obj);
			}
		};



		struct Appender
		{
			template<class Collection>
			static decltype(std::declval<Collection>().push_back(std::declval<typename Collection::value_type>()), void()) Append(Collection& col, const typename Collection::value_type& item)
			{
				col.push_back(item);
			}

			template<class Collection>
			static decltype(std::declval<Collection>().insert(std::declval<typename Collection::value_type>()), void()) Append(Collection& col, const typename Collection::value_type& item)
			{
				col.insert(item);
			}
		};
	}
}
#endif  // SMPS_STL_COLLECTIONS_SERIALIZER_H_
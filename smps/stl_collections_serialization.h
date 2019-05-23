#ifndef SMPS_STL_COLLECTIONS_SERIALIZER_H_
#define SMPS_STL_COLLECTIONS_SERIALIZER_H_
#include "serializer.h"
namespace smps {

	template<class T>
	serializable_object_type::CollectionType CheckSMPSObjectGhostFunc(std::vector<T> v);

	class StlCollectionAddProvider
	{
	public:
		template<class T>
		static void Add(std::vector<T>& obj, const T& item) {
			obj.push_back(item);
		}
	};


}
#endif  // SMPS_STL_COLLECTIONS_SERIALIZER_H_
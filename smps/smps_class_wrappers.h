#ifndef SMPS_CLASS_WRAPPERS_H_
#define SMPS_CLASS_WRAPPERS_H_

#include "serializable.h"
#include <utility>

namespace smps
{
	namespace class_wrappers
	{
		template<class FirstType, class SecondType>
		class pair :public smps::FieldComposition, public std::pair<FirstType, SecondType>
		{
		public:
			pair() {}
			pair(std::pair<FirstType, SecondType> stl_pair) :std::pair<FirstType, SecondType>(stl_pair) {}

			typedef std::pair<FirstType, SecondType> BasePair;
			SMPS_FIELDS(2)
			SMPS_FIELD_INH(1, first, BasePair);
			SMPS_FIELD_INH(2, second, BasePair);
		};

		template<class FirstType, class SecondType>
		static pair<FirstType, SecondType> make_pair(std::pair<FirstType, SecondType> stl_pair)
		{
			return pair<FirstType, SecondType>(stl_pair);
		}
	}
}

#endif  // SMPS_CLASS_WRAPPERS_H_

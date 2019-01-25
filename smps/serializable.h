#ifndef SMPS_SERIALIZABLE_H_
#define SMPS_SERIALIZABLE_H_


#include "smps_common.h"

#define SMPS_FIELDS(c) template<int n>\
class FieldAccessor;\
class FieldCount\
{\
public:\
	static const int value = c;\
};

#define SMPS_FIELD(i, name)template<> \
class FieldAccessor<i>\
{\
	public:\
		static std::string GetName(){return std::string(#name);}\
		template<class T>\
		static decltype(name) GetField(T* obj_ptr){return obj_ptr->name;}\
		template<class T>\
		static void SetField(T* obj_ptr, decltype(name)& value){ obj_ptr->name = value;}\
};\

namespace smps
{
	class SerializableField
	{};
}

#endif  // SMPS_SERIALIZABLE_H_
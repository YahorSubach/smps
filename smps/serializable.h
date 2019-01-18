#pragma once
#include "smps_common.h"
#include "serializer.h"


#define SMPS_FIELDS(c) template<int n>\
class field_accessor;\
class field_count\
{\
public:\
	static const int value = c;\
};

#define SMPS_FIELD(i, name)template<> \
class field_accessor<i>\
{\
	public:\
		static std::string GetName(){return std::string(#name);}\
		template<class T>\
		static decltype(name) GetField(T* obj_ptr){return obj_ptr->name;}\
};



namespace smps
{
	class serializable_field
	{};
}

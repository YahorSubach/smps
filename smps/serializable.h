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
		static const decltype(name)& GetField(const T& obj) {return obj.name;} \
		template<class T>\
		static void SetField(T& obj, const decltype(name)& value){ obj.name = value;}\
};\

#define SMPS_FIELD_EXT(i, name, getter, setter)template<> \
class FieldAccessor<i>\
{\
	public:\
		static std::string GetName(){return std::string(#name);}\
		template<class T>\
		static decltype(getter) GetField(T* obj_ptr){return obj_ptr->getter();}\
		template<class T>\
		static void SetField(T* obj_ptr, decltype(getter)& value){ obj_ptr->setter(value);}\
};\

#define SMPS_FIELD_INH(i, name, base)template<> \
class FieldAccessor<i>\
{\
	public:\
		static std::string GetName(){return std::string(#name);}\
		template<class T>\
		static decltype(base::name) GetField(T* obj_ptr){return obj_ptr->base::name;}\
		template<class T>\
		static void SetField(T* obj_ptr, decltype(base::name)& value){ obj_ptr->base::name = value;}\
};\




namespace smps
{
	class Serializable
	{};

	typedef Serializable Serializable;

	namespace serializable_object_type
	{
		class GeneralType {};
		class SpecialType {};
		class CollectionType {};
		class SerializableType {};
	}
}

#endif  // SMPS_SERIALIZABLE_H_
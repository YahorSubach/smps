#pragma once
namespace smps
{

	class serializer
	{
	public:
		/*template<int field_count>
		virtual serializationT Serialize(const serializable<field_count>& obj_for_serialization) = 0;

		template<class obj_type>
		virtual obj_type Deserialize(const serializationT& serialize_obj) = 0;*/
	};


	//template<class fieldNameT, class serializationT>
	//class serialization_object
	//{
	//public:
	//	virtual void AddSerializedField(const fieldNameT& field_name, const serializationT& field_serialized) = 0;
	//};

}
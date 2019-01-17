#pragma once

namespace smps
{

	template<class base_class, class field_class>
	class field
	{
		field_class
	};

	template<template<int n>>
	class ser
	{

	}



	class field_wrapper
	{
	public:

		template<class T>
		static field_wrapper Create(const T& field)
		{
			field_wrapper wrapper;
			wrapper.field_ptr = &(field);
			return wrapper;
		}

		template<class T>
		T get_field()
		{
			return *static_cast<T*>(field_ptr);
		}

	private:
		void* field_ptr;

	};

	class serialization_accumulator
	{
	public:
		template<class T>
		void add_field(const T& wrapped_field)
		{
			field_serializer<T>
		}
		
			virtual void add_field(const field_wrapper& wrapped_field) = 0;
	};
}

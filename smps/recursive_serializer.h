#pragma once
#include "serializer.h"
namespace smps
{

	template<class FieldContainerObjectType, int i>
	class RecursiveFieldsAggregator : public RecursiveFieldsAggregator<FieldContainerObjectType, i - 1>
	{
	public:
		template<class Accumulator, class Applicator>
		static void ProvideFieldsToAccumulator(Accumulator& accum, const FieldContainerObjectType& obj, bool iterationBegin = true)
		{
			RecursiveFieldsAggregator<FieldContainerObjectType, i - 1>::template ProvideFieldsToAccumulator<Accumulator, Applicator>(accum, obj, false);

			if (i != 1)
				Applicator::ContainerDecorator::DecorateContainerElementSplit(accum);

			std::string field_name = FieldContainerObjectType::FieldAccessor<i>::GetName();
			Applicator::ContainerDecorator::DecorateFieldHeader(accum, i, field_name);

			Serializer<Applicator>::Serialize(accum, FieldContainerObjectType::FieldAccessor<i>::GetField(&obj));

			if (iterationBegin)
				Applicator::ContainerDecorator::DecorateContainerEnd(accum);
		}
	};

	template<class FieldContainerObjectType>
	class RecursiveFieldsAggregator<FieldContainerObjectType, 0> {
	public:
		template<class Accumulator, class Applicator>
		static void ProvideFieldsToAccumulator(Accumulator& accum, const FieldContainerObjectType& obj, bool iterationBegin = true)
		{
			Applicator::ContainerDecorator::DecorateContainerBegin(accum);
		}
	};

	template<class FieldContainerObjectType, int i>
	class RecursiveFieldsSplitter : public RecursiveFieldsSplitter<FieldContainerObjectType, i - 1>
	{
	public:
		template<class Presentor, class Applicator>
		static void ExtractFieldsFromPresentor(Presentor& presentor, const FieldContainerObjectType& obj, bool iterationBegin = true)
		{
			RecursiveFieldsAggregator<FieldContainerObjectType, i - 1>::template ExtractFieldsFromPresentor<Presentor, Applicator>(presentor, obj, false);

			if (i != 1)
				Applicator::ContainerDecorator::UndecorateContainerElementSplit(accum);

			std::string field_name = FieldContainerObjectType::FieldAccessor<i>::GetName();
			if (Applicator::ContainerDecorator::UndecorateFieldHeader(presentor, i, field_name))
			{
				Serializer<Applicator>::Deserialize(presentor, FieldContainerObjectType::FieldAccessor<i>::GetField(&obj));
			}

			
			if (iterationBegin)
				Applicator::ContainerDecorator::UndecorateContainerEnd(accum);
		}
	};

	template<class FieldContainerObjectType>
	class RecursiveFieldsSplitter<FieldContainerObjectType, 0> {
	public:
		template<class Presentor, class Applicator>
		static void ExtractFieldsFromPresentor(Presentor& presentor, const FieldContainerObjectType& obj, bool iterationBegin = true)
		{
			Applicator::ContainerDecorator::UndecorateContainerBegin(presentor);
		}
	};


	template<class Accumulator, class Presentor, class AccumulatorInterpreter, class BaseTypeSerializer, class CollectionDecorator, class FieldsContainerDecorator>
	class ExplicitRecursiveSerializerApplicator
	{
	public:

		typedef ExplicitRecursiveSerializerApplicator<Accumulator, AccumulatorInterpreter, BaseTypeSerializer, CollectionDecorator, FieldsContainerDecorator> Applicator;
		typedef typename AccumulatorInterpreter::ResultType ResultType;
		typedef Accumulator AccumulatorType;
		typedef FieldsContainerDecorator ContainerDecorator;

		//Serialization methods

		template<class BaseType>
		static void SerializeBaseType(Accumulator& accum, BaseType& obj)
		{
			BaseTypeSerializer::Serialize(accum, obj);
		}
		template<class CollectionType>
		static void SerializeCollection(Accumulator& accum, CollectionType& collection)
		{
			CollectionDecorator::DecorateCollectionBegin(accum);
			for (auto it = collection.begin(); it != collection.end(); it++)
			{
				if (it != collection.begin())
					CollectionDecorator::DecorateCollectionElementSplit(accum);
				Serializer<Applicator>::Serialize(accum, *it);
			}
			CollectionDecorator::DecorateCollectionEnd(accum);
		}
		template<class FieldsContainerType>
		static void SerializeFieldsContainer(Accumulator& accum, FieldsContainerType& fieldsContainer)
		{
			RecursiveFieldsAggregator<FieldsContainerType, FieldsContainerType::FieldCount::value>::template ProvideFieldsToAccumulator<Accumulator, Applicator>(accum, fieldsContainer);
		}


		//Deserialization methods

		template<class BaseType>
		static void DeserializeBaseType(Presentor& presentor, BaseType& obj)
		{
			BaseTypeSerializer::Deserialize(presentor, obj);
		}
		template<class CollectionType>
		static void DeserializeCollection(Presentor& presentor, CollectionType& collection)
		{
			CollectionDecorator::UndecorateCollectionBegin(presentor);
			
			if (!CollectionDecorator::UndecorateCollectionEnd(presentor))
			{
				//TODO CollectionAdder
			}
			while (!CollectionDecorator::UndecorateCollectionEnd(presentor))
			{
				CollectionDecorator::UndecorateCollectionElementSplit(presentor);
				//TODO CollectionAdder 				Serializer<Applicator>::Deserialize(accum, *it);
			}			
		}
		template<class FieldsContainerType>
		static void DeserializeFieldsContainer(Presentor& presentor, FieldsContainerType& fieldsContainer)
		{
			RecursiveFieldsSplitter<FieldsContainerType, FieldsContainerType::FieldCount::value>::template ExtractFieldsFromPresentor<Presentor, Applicator>(presentor, fieldsContainer);
		}

	};
}
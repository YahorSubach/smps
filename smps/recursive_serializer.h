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
				Applicator::ContainerDecorator::SplitContainerElement(accum);

			std::string field_name = FieldContainerObjectType::FieldAccessor<i>::GetName();
			Applicator::ContainerDecorator::RegisterField(accum, i, field_name);

			Serializer<Applicator>::Serialize(accum, FieldContainerObjectType::FieldAccessor<i>::GetField(&obj));

			if (iterationBegin)
				Applicator::ContainerDecorator::EndContainer(accum);
		}
	};

	template<class FieldContainerObjectType>
	class RecursiveFieldsAggregator<FieldContainerObjectType, 0> {
	public:
		template<class Accumulator, class Applicator>
		static void ProvideFieldsToAccumulator(Accumulator& accum, const FieldContainerObjectType& obj, bool iterationBegin = true)
		{
			Applicator::ContainerDecorator::BeginContainer(accum);
		}
	};


	template<class Accumulator, class AccumulatorInterpreter, class BaseTypeSerializer, class CollectionDecorator, class FieldsContainerDecorator>
	class ExplicitRecursiveSerializerApplicator
	{
	public:

		typedef ExplicitRecursiveSerializerApplicator<Accumulator, AccumulatorInterpreter, BaseTypeSerializer, CollectionDecorator, FieldsContainerDecorator> Applicator;
		typedef typename AccumulatorInterpreter::ResultType ResultType;
		typedef Accumulator AccumulatorType;
		typedef FieldsContainerDecorator ContainerDecorator;


		template<class BaseType>
		static void SerializeBaseType(Accumulator& accum, BaseType& obj)
		{
			BaseTypeSerializer::Serialize(accum, obj);
		}
		template<class CollectionType>
		static void SerializeCollection(Accumulator& accum, CollectionType& collection)
		{
			CollectionDecorator::BeginCollection(accum);
			for (auto it = collection.begin(); it != collection.end(); it++)
			{
				if (it != collection.begin())
					CollectionDecorator::SplitCollectionElements(accum);
				Serializer<Applicator>::Serialize(accum, *it);
			}
			CollectionDecorator::EndCollection(accum);
		}
		template<class FieldsContainerType>
		static void SerializeFieldsContainer(Accumulator& accum, FieldsContainerType& fieldsContainer)
		{
			RecursiveFieldsAggregator<FieldsContainerType, FieldsContainerType::FieldCount::value>::template ProvideFieldsToAccumulator<Accumulator, Applicator>(accum, fieldsContainer);
		}
	};
}
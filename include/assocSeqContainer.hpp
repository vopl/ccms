#ifndef _ccms_assocSeqContainerMaker_hpp_
#define _ccms_assocSeqContainerMaker_hpp_

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	template <typename T1,typename T2>
	struct mutable_pair
	{
		typedef T1 first_type;
		typedef T2 second_type;

		mutable_pair():first(T1()),second(T2()){}
		mutable_pair(const T1& f,const T2& s):first(f),second(s){}
		mutable_pair(const std::pair<T1,T2>& p):first(p.first),second(p.second){}

		T1         first;
		mutable T2 second;
	};

	//////////////////////////////////////////////////////////////////////////
	//далее ассоциативный и упорядоченый контейнер на базе boost::multi_index
	template <
		class TKey, 
		class TValue,
		class TCompare = std::less<TKey>,
		class TAllocator = std::allocator<mutable_pair<TKey, TValue> >
	>
	struct AssocSeqContainer
		: public boost::multi_index::multi_index_container<
			mutable_pair<TKey, TValue>,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_unique<
					boost::multi_index::member<mutable_pair<TKey, TValue>, TKey, &mutable_pair<TKey, TValue>::first>, 
					TCompare
				>,
				boost::multi_index::sequenced<>
			>,
			TAllocator
		>

	{
		typedef TKey Key;
		typedef TValue Value;
		//элемент хранения
		typedef mutable_pair<Key, Value> Element;

		//буст контейнер

		

		typedef typename boost::multi_index::nth_index<AssocSeqContainer, 0>::type Assoc;
		typedef typename boost::multi_index::nth_index<AssocSeqContainer, 1>::type Seq;

		Assoc &assoc;
		Seq &seq;

		AssocSeqContainer()
			: assoc(AssocSeqContainer::template get<0>())
			, seq(AssocSeqContainer::template get<1>())
		{
		}
		~AssocSeqContainer()
		{
		}

		static Element mk(const Key &key, const Value &value)
		{
			return Element(key, value);
		}
	};




		//////////////////////////////////////////////////////////////////////////
	//далее ассоциативный и упорядоченый контейнер на базе boost::multi_index
	template <
		class TKey, 
		class TValue,
		class TCompare = std::less<TKey>,
		class TAllocator = std::allocator<mutable_pair<TKey, TValue> >
	>
	struct AssocNuRndContainer
		: public boost::multi_index::multi_index_container<
			mutable_pair<TKey, TValue>,
			boost::multi_index::indexed_by<
				boost::multi_index::ordered_non_unique<
					boost::multi_index::member<mutable_pair<TKey, TValue>, TKey, &mutable_pair<TKey, TValue>::first>, 
					TCompare
				>,
				boost::multi_index::random_access<>
			>,
			TAllocator
		>

	{
		typedef TKey Key;
		typedef TValue Value;
		//элемент хранения
		typedef mutable_pair<Key, Value> Element;

		//буст контейнер

		typedef typename boost::multi_index::nth_index<AssocNuRndContainer, 0>::type AssocNu;
		typedef typename boost::multi_index::nth_index<AssocNuRndContainer, 1>::type Rnd;

		AssocNu &assocNu;
		Rnd &rnd;

		AssocNuRndContainer()
			: assocNu(AssocNuRndContainer::template get<0>())
			, rnd(AssocNuRndContainer::template get<1>())
		{
		}
		~AssocNuRndContainer()
		{
		}

		static Element mk(const Key &key, const Value &value)
		{
			return Element(key, value);
		}
	};

}

#endif

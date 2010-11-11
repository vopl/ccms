#ifndef _ccms_tokenizer_hpp_
#define _ccms_tokenizer_hpp_

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	template <class Iterator>
	struct IteratorPair
	{
		Iterator _begin;
		Iterator _end;

		void assign(Iterator begin, Iterator end)
		{
			_begin = begin;
			_end = end;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	template <class Container>
	struct tokenizer
		: public boost::tokenizer<
			boost::char_separator<typename Container::value_type>, 
			typename Container::iterator,
			IteratorPair<typename Container::iterator> >
	{
		template <typename TDelim>
		tokenizer(typename Container::iterator begin, typename Container::iterator end, TDelim delim)
			: boost::tokenizer<
				boost::char_separator<typename Container::value_type>, 
				typename Container::iterator,
				IteratorPair<typename Container::iterator> > (begin, end, boost::char_separator<typename Container::value_type>(delim))
		{
		}

	};
}

#endif

#ifndef _ccms_select2nd_hpp_
#define _ccms_select2nd_hpp_

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	template<class P>
	struct select2nd
		: std::unary_function<P&, typename P::second_type>
	{
		typename P::second_type& operator()(P& p) const { return p.second; }
		const typename P::second_type& operator()(P const& p) const { return p.second; }
	};	
}

#endif

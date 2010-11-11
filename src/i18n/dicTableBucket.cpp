#include "stdafx.h"
#include "i18n/dicTableBucket.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	DicTableBucket::DicTableBucket()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	DicTableBucket::~DicTableBucket()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	void DicTableBucket::add(DicTablePtr dt)
	{
		_dts.push_back(dt);
	}

	//////////////////////////////////////////////////////////////////////////
	const char *DicTableBucket::translate(
		const char *id0)
	{
		const char *res = NULL;
		BOOST_FOREACH(DicTablePtr &dt, _dts)
		{
			res = dt->translate(id0);
			if(res != id0)
			{
				return res;
			}
		}

		return id0;
	}

	//////////////////////////////////////////////////////////////////////////
	const char *DicTableBucket::translate(
		const char *id0, const char *id1, 
		unsigned plural)
	{
		const char *res = NULL;
		BOOST_FOREACH(DicTablePtr &dt, _dts)
		{
			res = dt->translate(id0, id1, plural);
			if(res != id0 && res != id1)
			{
				return res;
			}
		}

		if(!res)
		{
			res = (1==plural)?id0:id1;
		}

		return res;
	}
}

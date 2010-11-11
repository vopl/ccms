#ifndef _ccms_dicTableBucket_hpp_
#define _ccms_dicTableBucket_hpp_

#include "i18n/dicTable.hpp"

namespace ccms
{
	class DicTableBucket
	{
		std::vector<DicTablePtr> _dts;
	public:
		DicTableBucket();
		~DicTableBucket();

		void add(DicTablePtr dt);

		const char *translate(
			const char *id0);
		const char *translate(
			const char *id0, const char *id1, 
			unsigned plural);
	};
	typedef boost::shared_ptr<DicTableBucket> DicTableBucketPtr;

}
#endif

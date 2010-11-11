#ifndef _ccms_dicTable_h_
#define _ccms_dicTable_h_

#include "i18n/pluralExpression.hpp"

namespace ccms
{
	//потоконебезопасный - 18 букв
	class DicTable
	{
		typedef std::map<std::string, std::string> TMIdValue;
		TMIdValue _mIdValue;

		unsigned _plurals;
		plural::ExpressionPtr _plural;

	public:
		DicTable();
		~DicTable();
		bool load(const std::string &file, bool replaceDoubles=false);
		bool merge(const DicTable &dt, bool replace=false);

		const char *translate(
			const char *id0);
		const char *translate(
			const char *id0, const char *id1, 
			unsigned plural);

	private:
		bool loadMo(std::istream &inf, TMIdValue &m);
		bool loadPo(std::istream &inf, TMIdValue &m);

		bool merge(const TMIdValue &m, bool replace=false);
	};
	typedef boost::shared_ptr<DicTable> DicTablePtr;
}

#endif

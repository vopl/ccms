#include "stdafx.h"
#include "tpm/templateEscaper.hpp"
#include "scripter/profiler.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	TemplateEscaper::TemplateEscaper(ETemplateEscaperType type, const TemplateEscaper *tail)
		: _type(type)
		, _tail(tail)
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	bool TemplateEscaper::print(std::ostream &out, const char *data, size_t size) const
	{
		//////////////////////////////////////////////////////////////////////////
#if USE_PROFILER
		ProfilerScopeHelper psh(g_profiler, NULL, "TemplateEscaper::print");
#endif
		//////////////////////////////////////////////////////////////////////////

		switch(_type)
		{
		case etetNull:
			return printWithTail(out, data, size);
		case etetTagName:
		case etetAttrName:
			//check for valid xml name and drop exception
			return printWithTail(out, data, size);
		case etetAttrValue:
			{
				const char *accumulerBegin = data;
				const char *cp = data, *end = data + size;
				for (; cp != end; ++cp)
				{
					switch (*cp)
					{
					case '"':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&quot;", 6)) return false;
						accumulerBegin = cp+1;
						break;
					case '<':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&lt;", 4)) return false;
						accumulerBegin = cp+1;
						break;
					case '&':
						printWithTail(out, accumulerBegin, cp-accumulerBegin);
						printWithTail(out, "&amp;", 5);
						accumulerBegin = cp+1;
						break;
					case '\n':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&#xA;", 5)) return false;
						accumulerBegin = cp+1;
						break;
					case '\r':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&#xD;", 5)) return false;
						accumulerBegin = cp+1;
						break;
					case '\t':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&#x9;", 5)) return false;
						accumulerBegin = cp+1;
						break;
					default:
						//accumulate
						break;
					}
				}
				if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
			}
			return true;
		case etetXml:
			{
				const char *accumulerBegin = data;
				const char *cp = data, *end = data + size;
				for (; cp != end; ++cp)
				{
					switch (*cp)
					{
					case '<':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&lt;", 4)) return false;
						accumulerBegin = cp+1;
						break;
					case '>':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&gt;", 4)) return false;
						accumulerBegin = cp+1;
						break;
					case '&':
						if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
						if(!printWithTail(out, "&amp;", 5)) return false;
						accumulerBegin = cp+1;
						break;
					default:
						//accumulate
						break;
					}
				}
				if(!printWithTail(out, accumulerBegin, cp-accumulerBegin)) return false;
			}
			return true;
		default:
			assert(0);
			return false;
		}
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	ETemplateEscaperType TemplateEscaper::getType() const
	{
		return _type;
	}
	
	//////////////////////////////////////////////////////////////////////////
	const TemplateEscaper *TemplateEscaper::getTail() const
	{
		return _tail;
	}

	//////////////////////////////////////////////////////////////////////////
	bool TemplateEscaper::printWithTail(std::ostream &out, const char *data, size_t size) const
	{
		//////////////////////////////////////////////////////////////////////////
#if USE_PROFILER
		ProfilerScopeHelper psh(g_profiler, NULL, "TemplateEscaper::printWithTail");
#endif
		//////////////////////////////////////////////////////////////////////////
		if(!size)
		{
			return true;
		}
		if(_tail)
		{
			return _tail->print(out, data, size);
		}
		out.write(data, size);
		return true;
	}

}

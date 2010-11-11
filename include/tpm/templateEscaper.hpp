#ifndef _ccms_templateEscaper_hpp_
#define _ccms_templateEscaper_hpp_


namespace ccms
{

	enum ETemplateEscaperType
	{
		etetNull,
		etetTagName,
		etetAttrName,
		etetAttrValue,
		etetXml,
	};
	//////////////////////////////////////////////////////////////////////////
	class TemplateEscaper
	{
		ETemplateEscaperType _type;
		const TemplateEscaper *_tail;
	public:
		TemplateEscaper(ETemplateEscaperType type, const TemplateEscaper *tail=0);
		bool print(std::ostream &out, const char *data, size_t size) const;
		ETemplateEscaperType getType() const;
		const TemplateEscaper *getTail() const;
	private:
		bool printWithTail(std::ostream &out, const char *data, size_t size) const;

	};
}
#endif

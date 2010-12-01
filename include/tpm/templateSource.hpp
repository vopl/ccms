#ifndef _ccms_templateSource_hpp_
#define _ccms_templateSource_hpp_

#include "tpm/templateEscaper.hpp"
#include "scripter/jsObject.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	class Template;

	//////////////////////////////////////////////////////////////////////////
	class TemplateSource;
	typedef boost::shared_ptr<TemplateSource> TemplateSourcePtr;

	//////////////////////////////////////////////////////////////////////////
	class TemplateSource
	{
		////////////////////
		Template *_owner;

		////////////////////
		ETemplateEscaperType _etet;

		////////////////////
		enum ETemplateSourceType
		{
			etstNull,
			etstText,
			etstJsval,
			etstProp,
		};
		ETemplateSourceType _type;

		////////////////////
		struct Text
		{
			char			*_data;
			size_t			_size;
		};
		////////////////////
		struct Prop
		{
			jsval			_idVal;
			jsid			_id;
			jsval			_val;
		};
		union Content
		{
			Text			_text;
			Prop			_prop;
			jsval			_jsval;
		} _content;

	public:
		struct PtrPred
			: public std::binary_function<TemplateSourcePtr, TemplateSourcePtr, bool>
		{	// functor for operator<
			bool operator()(const TemplateSourcePtr& _Left, const TemplateSourcePtr& _Right) const
			{	// apply operator< to operands
				return (_Left->less(*_Right));
			}
		};

	public:
		TemplateSource(Template *owner);
		~TemplateSource();

		void clearContent();
		void clearContentNoJs();
		bool setText(const char *data, size_t size);
		bool setJsval(jsval jsv, bool doSimplify);
		bool setProp(jsval id, jsval val);

		bool getJsval(JSObject *obj, jsval &jsv);

		void setEtet(ETemplateEscaperType etet);

		TemplateSourcePtr clone();

		bool print(JSObject *obj, std::ostream &out, const TemplateEscaper &escaper);

		bool less(const TemplateSource &with) const;
	private:
		bool printJsval(JSObject *obj, std::ostream &out, const TemplateEscaper &escaper, jsval jsv);

	};



}

#endif

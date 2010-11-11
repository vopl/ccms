#include "stdafx.h"
#include "i18n/dicTable.hpp"
#include "i18n/pluralExpressionBuilder.hpp"
#include "utils/ncvt.h"

#define MAXSIZE (1024*1024)

namespace ccms
{

	//////////////////////////////////////////////////////////////////////////
	DicTable::DicTable()
		: _plurals(0)
	{
	}

	//////////////////////////////////////////////////////////////////////////
	DicTable::~DicTable()
	{

	}

	//////////////////////////////////////////////////////////////////////////
	bool DicTable::load(const std::string &file, bool replaceDoubles)
	{
		std::ifstream inf(file.c_str(), std::ios::binary|std::ios::in);
		if(!inf)
		{
			return false;
		}

		TMIdValue m;

		unsigned char magicMo[4]={};
		inf.read((char *)magicMo, 4);
		if(0xde==magicMo[0] && 0x12==magicMo[1] && 0x04==magicMo[2] && 0x95==magicMo[3])
		{
			if(!loadMo(inf, m))
			{
				std::cerr<<"bad mo source "<<file<<std::endl;
				return false;
			}
		}
		else
		{
			inf.seekg(0);
			if(!loadPo(inf, m))
			{
				std::cerr<<"bad po source "<<file<<std::endl;
				return false;
			}
		}

		return merge(m, replaceDoubles);
	}

	//////////////////////////////////////////////////////////////////////////
	bool DicTable::merge(const DicTable &dt, bool replace)
	{
		return merge(dt._mIdValue, replace);
	}

	//////////////////////////////////////////////////////////////////////////
	const char *DicTable::translate(
		const char *id0)
	{
		TMIdValue::iterator iter = _mIdValue.find(id0);
		if(_mIdValue.end() == iter)
		{
			return id0;
		}

		return iter->second.c_str();
	}

	//////////////////////////////////////////////////////////////////////////
	const char *DicTable::translate(
		const char *id0, const char *id1, 
		unsigned plural)
	{
		assert(id1);
		std::string key(id0);
		key += '\0';
		key += id1;

		TMIdValue::iterator iter = _mIdValue.find(key);
		if(_mIdValue.end() == iter)
		{
			if(1!=plural)
			{
				return id1;
			}
			return id0;
		}

		if(_plural)
		{
			plural = _plural->eval(plural);
		}
		else
		{
			if(1!=plural)
			{
				return id1;
			}
			return id0;
		}
		assert(plural < _plurals);//формула вычислилась вне заявленного диапазона

		//find plural str in iter->second
		const std::string &value = iter->second;

		size_t pluralOffset = 0;
		while(plural-- && pluralOffset < value.size())
		{
			pluralOffset = value.find('\0', pluralOffset)+1;
		}
		if(pluralOffset < value.size())
		{
			return value.c_str() + pluralOffset;
		}

		//не хватает плурал переводов
		assert(0);
		return id1;
	}

	//////////////////////////////////////////////////////////////////////////
	bool DicTable::loadMo(std::istream &inf, TMIdValue &m)
	{
		struct MoHeader
		{
			boost::uint16_t _revision;
			boost::uint32_t _stringsAmount;
			boost::uint32_t _originalOffset;
			boost::uint32_t _translationOffset;
			boost::uint32_t _hashSize;
			boost::uint32_t _hashOffset;
		}header;

		inf.read((char *)&header, sizeof(header));
		if(inf.gcount() != sizeof(header))
		{
			//bad file;
			return false;
		}

		//читать строки
		std::vector<char> original, translation;

		for(boost::uint32_t i(0); i<header._stringsAmount; i++)
		{
			size_t originalOffset0 = header._originalOffset + i*8;
			inf.seekg(originalOffset0);
			if(originalOffset0 != inf.tellg())
			{
				return false;
			}

			boost::uint32_t originalPair[2];
			inf.read((char *)originalPair, 8);
			if(8 != inf.gcount())
			{
				return false;
			}
			if(originalPair[0] > MAXSIZE)
			{
				return false;
			}

			size_t translationOffset0 = header._translationOffset + i*8;
			inf.seekg(translationOffset0);
			if(translationOffset0 != inf.tellg())
			{
				return false;
			}

			boost::uint32_t translationPair[2];
			inf.read((char *)translationPair, 8);
			if(8 != inf.gcount())
			{
				return false;
			}
			if(translationPair[0] > MAXSIZE)
			{
				return false;
			}

			//////////////////////////////////////////////////////////////////////////
			inf.seekg(originalPair[1]);
			if(originalPair[1] != inf.tellg())
			{
				return false;
			}
			original.resize(originalPair[0]);
			if(originalPair[0])
			{
				inf.read(&original[0], originalPair[0]);
			}

			inf.seekg(translationPair[1]);
			if(translationPair[1] != inf.tellg())
			{
				return false;
			}
			translation.resize(translationPair[0]);
			if(translationPair[0])
			{
				inf.read(&translation[0], translationPair[0]);
			}

			m[std::string(original.begin(), original.end())] = std::string(translation.begin(), translation.end());

		}
		return true;

	}


	//////////////////////////////////////////////////////////////////////////
	bool DicTable::loadPo(std::istream &inf, TMIdValue &m)
	{
		using namespace boost::xpressive;

		boost::xpressive::cregex re = 
			*_s >> 
			repeat<0,1>(
				(s1=as_xpr("msgid"))|(s1="msgid_plural")|((s1=as_xpr("msgstr"))>>
				repeat<0,1>(as_xpr("[")>>(s2=+_d)>>"]"))
				) >> 
			*_s >> 
			as_xpr('"') >> (s3= -*_) >> '"' >> *_s >> eos;

		boost::xpressive::cregex reStr = 
			as_xpr('\\')>>as_xpr('n')
		;

		cmatch what;


		std::vector<char> line(MAXSIZE);

		//////////////////////////////////////////////////////////////////////////
		enum EIdType
		{
			eid_null,
			eid_id,
			eid_idPlural,
			eid_str,
			eid_strPlural,
		};

		struct Accumuler{
			enum EState
			{
				es_null,
				es_id,
				es_value,
			} _state;
			std::string _id;
			std::string _value;
			TMIdValue & _mIdValue;

			Accumuler(TMIdValue &mIdValue)
				: _state(es_null)
				, _mIdValue(mIdValue)
			{}
			void push(EIdType idType, int nplural, const std::string &value)
			{
				if(eid_id == idType)
				{
					flush();
				}

				switch(_state)
				{
				case es_null:
					switch(idType)
					{
					case eid_id:
						decode(value, _id);
						_state = es_id;
						break;
					}
					break;
				case es_id:
					switch(idType)
					{
					case eid_null:
						decode(value, _id);
						break;
					case eid_idPlural:
						_id += '\0';
						decode(value, _id);
						break;
					case eid_strPlural:
					case eid_str:
						decode(value, _value);
						_state = es_value;
						break;
					default:
						flush();
						push(idType, nplural, value);
					}
					break;
				case es_value:
					switch(idType)
					{
					case eid_null:
						decode(value, _value);
						break;
					case eid_strPlural:
						if(nplural>0)
						{
							_value += '\0';
						}
						//no break
					case eid_str:
						decode(value, _value);
						break;
					default:
						flush();
						push(idType, nplural, value);
					}
					break;
				}

			}
			void flush()
			{
				if(es_value == _state)
				{
					_mIdValue[_id] = _value;
				}
				_id.clear();
				_value.clear();
				_state = es_null;
			}
			void decode(const std::string &str, std::string &dst)
			{
				for(size_t i(0); i<str.size(); i++)
				{
					if('\\' == str[i])
					{
						if(str.size() > i+1)
						{
							switch(str[i+1])
							{
							case 'a': dst += '\a';break;
							case 'b': dst += '\b';break;
							case 'f': dst += '\f';break;
							case 'n': dst += '\n';break;
							case 'r': dst += '\r';break;
							case 't': dst += '\r';break;
							case 'v': dst += '\v';break;
							case '"': dst += '"';break;
							case '\'': dst += '\'';break;
							default:
								dst += '\\';
								dst += str[i+1];
							}
							i++;
						}
						else
						{
							dst += str[i];
						}
					}
					else
					{
						dst += str[i];
					}
				}
			}
		};
		Accumuler accumuler(m);

		while(inf)
		{
			inf.getline(&line[0], MAXSIZE-1);
			size_t len = inf.gcount();
			line[len] = 0;
			if(len)
			{
				if(regex_match(&line[0], what, re))
				{
					const std::string &id = what[1];
					const std::string &nplural = what[2];
					const std::string &value = what[3];

					EIdType t = eid_null;
					int np = -1;
					if(id.size())
					{
						if(id == "msgid") t = eid_id;
						else if(id == "msgid_plural") t = eid_idPlural;
						else if(id == "msgstr")
						{
							if(nplural.empty())
							{
								t = eid_str;
							}
							else
							{
								np = _atoi(nplural.c_str());
								t = eid_strPlural;
							}
						}
					}
					accumuler.push(t, np, value);
				}
			}
		}
		accumuler.flush();
		return true;
	}




	//////////////////////////////////////////////////////////////////////////
	bool DicTable::merge(const TMIdValue &m, bool replace)
	{
		bool needParsePlural = !_plural;

		if(replace)
		{
			BOOST_FOREACH(const TMIdValue::value_type &v, m)
			{
				_mIdValue[v.first] = v.second;
			}
		}
		else
		{
			_mIdValue.insert(m.begin(), m.end());
			needParsePlural = true;
		}

		if(needParsePlural && _mIdValue.end()!=_mIdValue.find(""))
		{
			//update plural formula
			std::string &sys = _mIdValue[""];

			unsigned plurals;
			std::string plural;

			using namespace boost::spirit::classic;
			rule<> line = 
				*((eps_p-as_lower_d[str_p("plural-forms:")]) >> *(anychar_p-eol_p) >> eol_p)>>

				as_lower_d[str_p("plural-forms:")] >> *space_p >>
				str_p("nplurals=") >> uint_p[assign_a(plurals)] >> ';' >> *space_p >>
				str_p("plural=") >> 
				(*(anychar_p-';'))[
						assign_a(plural)
				] >> ';' >> *space_p >>
				*anychar_p;

			bool full = parse(sys.c_str(), sys.c_str()+sys.size(), line).full;
			if(full)
			{
				_plural = plural::ExpressionBuilder().parse(plural.c_str(), plural.c_str()+plural.size());
				if(_plural)
				{
					_plurals = plurals;
				}
				else
				{
					_plurals = 0;
				}
			}

		}
// 		if(_plural)
// 		{
// 			for(size_t i(0); i<200; i++)
// 			{
// 				size_t r = _plural->eval(i);
// 				std::cout<<i<<","<<r<<std::endl;
// 			}
// 		}
// 		exit(0);

		return true;
	}

	static DicTable dt;
}


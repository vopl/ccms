#include "stdafx.h"
#include "struct/propertyInstance.hpp"
#include "router/request.hpp"
#include "router/router.hpp"

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	PropertyInstance::PropertyInstance()
		: _executable(true)
		, _traceFile(true)
		, _fileMTime(0)
		, _function(nullFunction)
		, _parent(NULL)
		, JsObject(true, "PropertyInstance")
	{
		//////////////////////////////////////////////////////////////////////////
		jsRegisterMeth("setKind",			&PropertyInstance::call_setKind, 1);
		jsRegisterMeth("setValue",			&PropertyInstance::call_setValue, 1);
		jsRegisterMeth("setDescription",	&PropertyInstance::call_setDescription, 1);
		jsRegisterMeth("setExecutable",		&PropertyInstance::call_setExecutable, 1);
		jsRegisterMeth("setFile",			&PropertyInstance::call_setFile, 1);
		jsRegisterMeth("setTraceFile",		&PropertyInstance::call_setTraceFile, 1);
	}

	//////////////////////////////////////////////////////////////////////////
	PropertyInstance::~PropertyInstance()
	{
	}
	
	//////////////////////////////////////////////////////////////////////////
	void PropertyInstance::init()
	{
		if(isInited())
		{
			return;
		}

		update();

		JSContext *cx = ecx()->_jsCx;

		//////////////////////////////////////////////////////////////////////////
		(JSExceptionReporter)jsRegisterProp("kind", _kind);
		(JSExceptionReporter)jsRegisterProp("value", _value);
		(JSExceptionReporter)jsRegisterProp("description", _description);
		(JSExceptionReporter)jsRegisterProp("executable", _executable?JSVAL_TRUE:JSVAL_FALSE);
		(JSExceptionReporter)jsRegisterProp("file", _file.string());
		(JSExceptionReporter)jsRegisterProp("traceFile", _traceFile?JSVAL_TRUE:JSVAL_FALSE);

		JsObject *parent = dynamic_cast<JsObject *>(_parent);
		if(parent)
		{
			(JSExceptionReporter)jsRegisterProp("parent", parent->thisJsval());
		}

		Initializable::init();
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	void PropertyInstance::deinit()
	{
		if(!isInited())
		{
			return;
		}

		Initializable::deinit();

		dropFunction();

		deletePermanent("kind");
		deletePermanent("value");
		deletePermanent("description");
		deletePermanent("executable");
		deletePermanent("file");
		deletePermanent("traceFile");
		deletePermanent("parent");
	}

	//////////////////////////////////////////////////////////////////////////
	void PropertyInstance::setParent(PropertyInstanceParent *parent)
	{
		_parent = parent;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInstance::render(JsObject *owner, uintN argc, jsval *argv, jsval *rval)
	{
		update();
		if(_executable)
		{
			if(!_function)
			{
				Request *request = ecx()->_request;
				if(request)
				{
					request->out<<"[no compiled function at "<<(_parent?_parent->propertyName4Error(this):" unknownPropertyParent")<<"]";
				}
				else
				{
					std::cout<<"[no compiled function at "<<(_parent?_parent->propertyName4Error(this):" unknownPropertyParent")<<"]";
				}
				*rval = JSVAL_VOID;
				return true;
			}
			assert(owner);
			*rval = ecx()->_scripter->f_exec(_function, owner->thisJsobj(), argc, argv);
			return true;
		}
		else
		{
			*rval = STRING_TO_JSVAL(JS_NewStringCopyN(ecx()->_jsCx, _value.data(), _value.size()));
			return true;
		}
		*rval = JSVAL_VOID;
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInstance::call_setKind(uintN argc, jsval *argv, jsval *rval)
	{
		if(1>argc)
		{
			JS_ReportError(ecx()->_jsCx, "[PropertyInstance.setKind must be called with 1 argument]");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;
		_kind = s;
		reinit();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInstance::call_setValue(uintN argc, jsval *argv, jsval *rval)
	{
		if(1>argc)
		{
			JS_ReportError(ecx()->_jsCx, "[PropertyInstance.setValue must be called with 1 argument]");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;
		_value = s;
		reinit();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInstance::call_setDescription(uintN argc, jsval *argv, jsval *rval)
	{
		if(1>argc)
		{
			JS_ReportError(ecx()->_jsCx, "[PropertyInstance.setDescription must be called with 1 argument]");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;
		_description = s;
		reinit();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInstance::call_setExecutable(uintN argc, jsval *argv, jsval *rval)
	{
		if(1>argc)
		{
			JS_ReportError(ecx()->_jsCx, "[PropertyInstance.setExecutable must be called with 1 argument]");
			return false;
		}

		JSBool b;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &b))return false;
		_executable = b?true:false;
		reinit();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInstance::call_setFile(uintN argc, jsval *argv, jsval *rval)
	{
		if(1>argc)
		{
			JS_ReportError(ecx()->_jsCx, "[PropertyInstance.setFile must be called with 1 argument]");
			return false;
		}

		char *s;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "s", &s))return false;
		_file = s;
		_fileMTime = 0;
		reinit();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	bool PropertyInstance::call_setTraceFile(uintN argc, jsval *argv, jsval *rval)
	{
		if(1>argc)
		{
			JS_ReportError(ecx()->_jsCx, "[PropertyInstance.setTraceFile must be called with 1 argument]");
			return false;
		}

		JSBool b;
		if(!JS_ConvertArguments(ecx()->_jsCx, 1, argv, "b", &b))return false;
		_traceFile = b?true:false;
		reinit();
		return true;
	}

	//////////////////////////////////////////////////////////////////////////
	void PropertyInstance::update(bool force)
	{
		bool needRead = !_file.empty();

#ifdef NDEBUG
		if(needRead && _fileMTime)
		{
			needRead = false;
		}
#endif

		//следить время модификации файла
		if(needRead && !force && _traceFile)
		{
			//тут тормоз
			time_t mt = boost::filesystem::last_write_time(_file);
			if(_fileMTime >= mt)
			{
				needRead = false;
			}
		}
	

		//грузить содержимое файла
		if(needRead)
		{
			//форс или трассировка 
			std::ifstream in(_file.string().c_str(), std::ios::binary);
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			in.seekg(0, std::ios::beg);
			_value.resize(size);
			in.read(const_cast<char *>(_value.data()), size);
			in.close();

			_fileMTime = boost::filesystem::last_write_time(_file);
		}

		if(_executable)
		{
			if(needRead || !_function || force)
			{
				std::string name4Error = _file.string();
// 				if(_parent)
// 				{
// 					name4Error += "("+_parent->propertyName4Error(this)+")";
// 				}
				if(name4Error.empty())
				{
					name4Error = _parent->propertyName4Error(this);
					name4Error += ".js";
				}

				dropFunction();
				_function = ecx()->_scripter->f_compile(_value, name4Error);

// 				//бросить исходник, он все равно не будет использоваться
// 				if(_function)
// 				{
// 					_value.swap(std::string());
// 				}
			}
		}
		else
		{
			dropFunction();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void PropertyInstance::dropFunction()
	{
		if(_function)
		{
			ecx()->_scripter->f_drop(_function);
			_function = nullFunction;
		}
	}
}

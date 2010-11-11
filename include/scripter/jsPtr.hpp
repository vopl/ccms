#ifndef _ccms_jsPtr_h_
#define _ccms_jsPtr_h_

namespace ccms
{
	//////////////////////////////////////////////////////////////////////////
	template <class T>
	class JsPtr
	{
		T *_p;

	public:
		typedef T value_type;

	public:
		JsPtr()
			: _p(NULL)
		{
		}

		JsPtr(T *p)
			: _p(p)
		{
			if(_p) _p->incRef();
		}
		template<class Y> JsPtr(Y *p)
			: _p(p)
		{
			if(_p) _p->incRef();
		}

		JsPtr(const JsPtr<T> &p)
			: _p(p._p)
		{
			if(_p) _p->incRef();
		}

		template<class Y> JsPtr(const JsPtr<Y> &p)
			: _p(p.get())
		{
			if(_p) _p->incRef();
		}

		//////////////////////////////////////////////////////////////////////////
		~JsPtr()
		{
			reset();
		}

		//////////////////////////////////////////////////////////////////////////
		JsPtr<T> &operator=(T *p)
		{
			if(_p) _p->decRef();
			_p = p;
			if(_p) _p->incRef();
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		JsPtr<T> &operator=(JsPtr<T> const &p)
		{
			if(_p) _p->decRef();
			_p = p.get();
			if(_p) _p->incRef();
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		template <class Y> JsPtr<T> &operator=(Y *p)
		{
			if(_p) _p->decRef();
			_p = p;
			if(_p) _p->incRef();
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		template <class Y> JsPtr<T> &operator=(JsPtr<Y> const &p)
		{
			if(_p) _p->decRef();
			_p = p;
			if(_p) _p->incRef();
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////
		void reset()
		{
			if(_p) _p->decRef();
			_p = NULL;
		}

		//////////////////////////////////////////////////////////////////////////
		T *get()
		{
			return _p;
		}

		//////////////////////////////////////////////////////////////////////////
		T *get() const
		{
			return _p;
		}

		//////////////////////////////////////////////////////////////////////////
		T *operator->()
		{
			assert(_p);
			return _p;
		}

		//////////////////////////////////////////////////////////////////////////
		T *operator->() const
		{
			assert(_p);
			return _p;
		}

		//////////////////////////////////////////////////////////////////////////
		T &operator*()
		{
			assert(_p);
			return *_p;
		}

		//////////////////////////////////////////////////////////////////////////
		T &operator*() const
		{
			assert(_p);
			return *_p;
		}

		//////////////////////////////////////////////////////////////////////////
		operator bool()const
		{
			return _p?true:false;
		}

		//////////////////////////////////////////////////////////////////////////
		bool operator!()const
		{
			return !(bool)*this;
		}
	};
}

#endif

#ifndef _ccms_initializable_hpp_
#define _ccms_initializable_hpp_

namespace ccms
{

	class Initializable
	{
	private:
		bool _isInited;

	public:
		Initializable()
			: _isInited(false)
		{
		}

		virtual ~Initializable()
		{
			deinit();
		}

		bool isInited()
		{
			return _isInited;
		}

		virtual void init()
		{
			_isInited = true;
		}

		virtual void deinit()
		{
			if(_isInited)
			{
				_isInited = false;
			}
		}


		virtual void reinit()
		{
			if(_isInited)
			{
				deinit();
				init();
			}
		}
	};
}

#endif

let t = arguments.callee.t;


if(!t)
{
	t = 
	{
		print:function()
		{
			request.pushHeader('Content-Type', "application/xhtml+xml; charset=utf-8");
			print('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">\n');
		}
	}
	arguments.callee.t = t;
}

return t;
let t = arguments.callee.t;


if(!t)
{
	t = 
	{
		print:function()
		{
			print('<!-- epilog -->');
		}
	}
	arguments.callee.t = t;
}

return t;
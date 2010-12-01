let t = arguments.callee.t;


if(!t)
{
	t = 
	{
		print:function()
		{
			switch(ui.agent.type)
			{
			case 'xhtml':
				request.pushHeader('Content-Type', "application/xhtml+xml; charset=utf-8");
				print('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">\n');
				break;
			case 'html':
				request.pushHeader('Content-Type', "text/html; charset=utf-8");
				print('<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">\n');
				break;
			default:
				request.pushHeader('Content-Type', ui.agent.type+"; charset=utf-8");
				print('<!-- prolog for type  '+esc.xml(ui.agent.type)+'-->\n');
			}
		}
	}
	arguments.callee.t = t;
}

return t;
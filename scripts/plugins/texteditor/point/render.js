let mode = arguments[0];
let res = arguments[1];

switch(mode)
{
case 'render':
	dump([mode, res]);
	break;
case 'ping':
	dump([mode, res]);
	break;
case 'set':
	dump([mode, res]);
	break;
case 'setImage':
	dump([mode, res]);
	break;
case 'setFile':
	dump([mode, res]);
	break;
case 'setContent':
	dump([mode, res]);
	break;
case 'reset':
	dump([mode, res]);
	break;
case 'getContent':
	dump([mode, res]);
	break;
case 'drop':
	dump([mode, res]);
	break;
default:
	request.setStatusCode(501);//Not Implemented
	return;
}


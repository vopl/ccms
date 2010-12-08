let mode = arguments[0];
let res = arguments[1];

switch(mode)
{
case 'setImage':
	dumps([mode, res]);
	break;
case 'setFile':
	dumps([mode, res]);
	break;
case 'setContent':
	dumps([mode, res]);
	break;
case 'reset':
	dumps([mode, res]);
	break;
case 'getContent':
	dumps([mode, res]);
	break;
case 'drop':
	dumps([mode, res]);
	break;
default:
	request.setStatusCode(501);//Not Implemented
	return;
}


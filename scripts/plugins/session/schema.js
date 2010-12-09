var orm = arguments[0];
var install = arguments[1];

if(!install)
{
	orm.delCategory('Session');
	return;
}


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'Session',
	fields:
	{
		id:'string',

		ctime:'date',
		atime:'date',
		dtime:'date',
		remember:'boolean',
		
		data:'string',
		
		addr:'string',
		
		secret:'string',
	},
});


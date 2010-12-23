let field = arguments[0];

let impl = 
{
	dbTypeSet:'character varying',
	dbTypeCheck:'character varying',

	db2orm:function(v)
	{
		try
		{
			v = JSON.parse(v);
		}
		catch(e)
		{
			warn('orm driver json.db2orm: '+e);
		}

		return v;
	},
	orm2db:function(v)
	{
		try
		{
			v = JSON.stringify(v);
		}
		catch(e)
		{
			warn('orm driver json.orm2db: '+e);
		}

		return v;
	},
};
impl.__proto__ = field;
return impl;

let field = arguments[0];

let impl = 
{
	dbTypeSet:'character varying',
	dbTypeCheck:'character varying',
};
impl.__proto__ = field;
return impl;

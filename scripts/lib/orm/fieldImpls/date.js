let field = arguments[0];

let impl = 
{
	dbTypeSet:'timestamp without time zone',
	dbTypeCheck:'timestamp without time zone',
};
impl.__proto__ = field;
return impl;

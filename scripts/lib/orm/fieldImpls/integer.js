let field = arguments[0];

let impl = 
{
	dbTypeSet:'integer',
	dbTypeCheck:'integer',
};
impl.__proto__ = field;
return impl;


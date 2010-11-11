let field = arguments[0];

let impl = 
{
	dbTypeSet:'boolean',
	dbTypeCheck:'boolean',
};
impl.__proto__ = field;
return impl;


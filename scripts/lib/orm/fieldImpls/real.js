let field = arguments[0];

let impl = 
{
	dbTypeSet:'double precision',
	dbTypeCheck:'double precision',
};
impl.__proto__ = field;
return impl;

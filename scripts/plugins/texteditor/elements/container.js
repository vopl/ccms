let e = {};

///////////////////////////////////////////////
e.kind = 'container';
e.childs = [];

///////////////////////////////////////////////
e.render = function render(mode)
{
	if('childs' in this)
	{
		let t = arguments.callee.t;
		if(!t)
		{
			t = router.createTemplate();
			t.compile(<>{t.childs}</>);
			arguments.callee.t = t;
		}
		t = t.clone();
		t.childs = this.childs.map(function(v) v.render(mode));
		return t;
	}

	let te = arguments.callee.te;
	if(!te)
	{
		te = router.createTemplate();
		te.compile('');
		arguments.callee.te = te;
	}
	return te;
}


e.__proto__ = exec('super.js');
return e;
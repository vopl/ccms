let tag = arguments[0];
let content = arguments[1];
let attributes = arguments[2];

let attributesAmount = 0;
if(attributes) for(let an in attributes) attributesAmount++;


let t;
if(undefined===content || null===content || ''===content)
{
	t = arguments.callee.tnc;
	if(!t)
	{
		t = [];
		arguments.callee.tnc = t;
	}
	while(attributesAmount >= t.length)
	{
		let arity = t.length;
		let subt = router.createTemplate();
		let src = <{subt.tag}/>;
		for(var cnt=1; cnt<=arity; cnt++)
		{
			src.@[subt['an'+cnt]] = subt['av'+cnt];
		}
		subt.compile(src);
		t.push(subt);
	}

	t = t[attributesAmount].clone();
}
else
{
	t = arguments.callee.tc;
	if(!t)
	{
		t = [];
		arguments.callee.tc = t;
	}
	while(attributesAmount >= t.length)
	{
		let arity = t.length;
		let subt = router.createTemplate();
		let src = <{subt.tag}>{subt.content}</{subt.tag}>;
		for(var cnt=1; cnt<=arity; cnt++)
		{
			src.@[subt['an'+cnt]] = subt['av'+cnt];
		}
		subt.compile(src);
		t.push(subt);
	}

	t = t[attributesAmount].clone();
	t.content = content;
}

if(attributes) 
{
	let cnt=1;
	for(let an in attributes)
	{
		t['an'+cnt]=an;
		t['av'+cnt]=attributes[an];
		cnt++;
	}
}

t.tag = tag;

return t;
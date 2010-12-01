let t = arguments.callee.t;
if(!t)
{
	t = [];
	arguments.callee.t = t;
}

let tag = arguments[0];
let content = arguments[1];
let attributes = arguments[2];

let attributesAmount = 0;
if(attributes) for(let an in attributes) attributesAmount++;

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
t.content = content;

return t;
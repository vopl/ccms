////////////////////////////////////////////////////////////
//массиву back и  front
Array.prototype.__defineGetter__('back', function()
{
	if(!this.length) return undefined;
	return this[this.length-1];
});
Array.prototype.__defineSetter__('back', function(v)
{
	if(!this.length) this[0] = v;
	else this[this.length-1]=v;
});
he(Array.prototype, 'back');





Array.prototype.__defineGetter__('front', function()
{
	if(!this.length) return undefined;
	return this[0];
});
Array.prototype.__defineSetter__('front', function(v)
{
	this[0] = v;
});
he(Array.prototype, 'front');



function dn(v, n)
{
	v = String(v);
	while(v.length < n) v = '0'+v;
	return v;
}


//////////////////////////////////////////////////////////
//для даты временной штамп
Date.prototype.__defineGetter__('ts', function()
{
	return ["",
		dn(this.getFullYear(), 4),
		"-",
		dn(this.getMonth()+1, 2),
		"-",
		dn(this.getDate(), 2),
		" ",
		dn(this.getHours(), 2),
		":",
		dn(this.getMinutes(), 2),
		":",
		dn(this.getSeconds(), 2),
		"",
	].join('');
});



Date.prototype.__defineSetter__('ts', function(v)
{
	let r = /(\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})/.exec(v);
	if(!r)
	{
		throw Error("Bad value for Date.ts: "+v);
	}

	this.setYear(r[1]);
	this.setMonth(r[2]-1);
	this.setDate(r[3]);
	this.setHours(r[4]);
	this.setMinutes(r[5]);
	this.setSeconds(r[6]);
	this.setMilliseconds(0);
});

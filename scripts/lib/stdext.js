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



//////////////////////////////////////////////////////////
//массиву добавлялка ключей или значений из хэша
Array.prototype.pushKeys = function pushKeys(obj)
{
	for(let k in obj) this.push(k);
	return this;
};
he(Array.prototype, 'pushKeys');

Array.prototype.pushValues = function pushValues(obj)
{
	for each(let k in obj) this.push(k);
	return this;
};
he(Array.prototype, 'pushValues');



//////////////////////////////////////////////////////////
//массиву раммежевалка типа join только без приведения к строке
Array.prototype.dilute = function dilute(water)
{
	let res = [];
	if(!this.length) return res;
	for(let i=0; i<this.length-1; i++)
	{
		res.push(this[i]);
		res.push(water);
	}
	res.push(this.back);
	return res;
};
he(Array.prototype, 'dilute');






//////////////////////////////////////////////////////////
//для даты временной штамп
function dn(v, n)
{
	v = String(v);
	while(v.length < n) v = '0'+v;
	return v;
}


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

//////////////////////////////////////////////////////////
//для даты временной штамп
Date.prototype.__defineGetter__('tsd', function()
{
	return ["",
		dn(this.getFullYear(), 4),
		"-",
		dn(this.getMonth()+1, 2),
		"-",
		dn(this.getDate(), 2),
		"",
	].join('');
});



Date.prototype.__defineSetter__('tsd', function(v)
{
	let r = /(\d{4})-(\d{2})-(\d{2})/.exec(v);
	if(!r)
	{
		throw Error("Bad value for Date.tsd: "+v);
	}

	this.setYear(r[1]);
	this.setMonth(r[2]-1);
	this.setDate(r[3]);
	this.setHours(0);
	this.setMinutes(0);
	this.setSeconds(0);
	this.setMilliseconds(0);
});

Date.fromTs = function(ts)
{
	let res = new Date();
	res.ts = ts;
	return res;
}

Date.fromTsd = function(tsd)
{
	let res = new Date();
	res.tsd = tsd;
	return res;
}

Date.new = function()
{
	return new Date();
}

if(!arguments.callee.stat1)
{
	arguments.callee.stat1 = {};
	arguments.callee.stat2 = {};
	arguments.callee.cnt = {a:0};
	
	let stat1 = arguments.callee.stat1;
	let stat2 = arguments.callee.stat2;
	for(let k=0; k<10000; k++)
	{
		let v = hash.sha1_(k, Math.random());
		if(!stat1[v.charAt(5)]) stat1[v.charAt(5)] = 0;
		stat1[v.charAt(5)]++;

		if(!stat2[v.charAt(3)]) stat2[v.charAt(3)] = 0;
		stat2[v.charAt(3)]++;
	}
	
}
let stat1 = arguments.callee.stat1;
let stat2 = arguments.callee.stat2;
let cnt = arguments.callee.cnt;
for(let k=0; k<10000; k++)
{
	let v = hash.sha1_(k, Math.random());
	for(let j=0; j<v.length-1; j++)
	{
		stat1[v.charAt(j)]++;
	}
	stat2[v.charAt(v.length-1)]++;
}
cnt.a += 10000

return dumps([cnt.a, stat1, stat2]);

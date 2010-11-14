//return <div>data is not setted at {this.path}</div>;
let name = this.properties.placeName();
if(name in ui.blocks)
{
	let src = ui.blocks[name];
	if(!(src instanceof Array))
	{
		src = [src];
	}
	let res = src.map(function(entry)	
		{
			if('object' == typeof entry)
			{
				if('priority' in entry && 'content' in entry) return entry;
			}
			return {
				priority:0,
				content:entry,
			};
		});
		
	function pred(a, b)
	{
		if(a.priority < b.priority)	return 1;
		if(a.priority > b.priority)	return -1;
		return 0;
	}
	res.sort(pred);
	
	res = res.map(function(entry){return entry.content});
	
	return res;
}
return undefined;


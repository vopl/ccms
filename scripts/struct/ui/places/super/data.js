//return <div>data is not setted at {this.path}</div>;
let name = this.properties.placeName();
if(name in ui.blocks)
{
	let src = ui.blocks[name];
	if(!(src instanceof Array))
	{
		src = [src];
	}
	let srca = [];
	for each(let v in src) srca.push(v);
	src = srca;

	let res = src.map(function(entry)	
		{
			if(entry instanceof CppObject && 'BlockEntry' == entry.cppClass)
			{
				return entry;
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
	
	let point = this;
	res = res.map(function(entry){return point.properties.wrapperElement(entry.content)});
	
	return res;
}
return undefined;


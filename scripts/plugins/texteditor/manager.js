let manager = {};

///////////////////////////////////////
manager.tes = {};

///////////////////////////////////////
manager.setTe = function(teid, options)
{
	if(options)
	{
		if(teid in this.tes)
		{
			warn('texteditor.manager.setTe: replace te '+teid);
		}

		this.tes[teid] = exec('te.js', this, options);
	}
	else
	{
		delete this.tes[teid];
	}
}
///////////////////////////////////////
manager.getInstance = function getInstance(teid, sid, uid)
{
	if(!(teid in this.tes))
	{
		return undefined;
	}
	
	let te = this.tes[teid];
	
	return {};//
}

return manager;
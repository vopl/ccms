let manager = {};

///////////////////////////////////////
manager.tes = {};

///////////////////////////////////////
manager.instances = {};
manager.baseInstance = exec('baseInstance.js');

///////////////////////////////////////
manager.setTe = function(teid, options)
{
	if(options)
	{
		if(teid in this.tes)
		{
			warn('texteditor.manager.setTe: replace te '+teid);
		}

		this.tes[teid] = exec('te.js', this, teid, options);
	}
	else
	{
		if(teid in this.tes)
		{
			delete this.tes[teid];
		}
	}

}

///////////////////////////////////////
manager.getTe = function(teid, options)
{
	if(!(teid in this.tes))
	{
		if(options)
		{
			this.setTe(teid, options);
			return this.tes[teid];
		}
		else
		{
			return undefined;
			//throw Error("manager.getTe: absent te and no options specified for create one");
		}
	}

	return this.tes[teid];
}


///////////////////////////////////////
manager.genIsid = function genIsid(teid, did)
{
	if(!teid) throw Error("manager.genIsid: teid must be setted");
	if('object' == typeof(teid)) teid = teid.id;
	if(!(teid in this.tes)) throw Error("manager.genIsid: invalud teid");

	let iKey = 
	{
		teid:teid,
		did:did,
	};
	
	let pswd = router.cd.global.session.secret;
	let isid = crypto.aes.encodeJson(pswd, iKey);
	return isid;
}

///////////////////////////////////////
manager.getInstance = function getInstance(isid)
{
	let pswd = router.cd.global.session.secret;

	let iKey = crypto.aes.decodeJson(pswd, isid);

	if(	!iKey || 
		'object' != typeof(iKey) ||
		!(iKey.teid in this.tes))
	{
		return undefined;
	}


	let te = this.tes[iKey.teid];

	if(!(isid in this.instances))
	{
		this.instances[isid] = this.mkInstance(isid, te, iKey.did, undefined);
	}

	//assert(this.instances[isid].id == isid)
	//assert(this.instances[isid].te.id == iKey.teid)

	return this.instances[isid];
}

///////////////////////////////////////////
manager.pingInstance = function pingInstance(instance)
{
	//...
}

///////////////////////////////////////////
manager.dropInstance = function dropInstance(instanceOrIsid)
{
	if(typeof(instanceOrIsid) == 'object')
	{
		instanceOrIsid = instanceOrIsid.isid;
	}
	if(instanceOrIsid in this.instances)
	{
		delete this.instances[instanceOrIsid];
	}
}

///////////////////////////////////////////
manager.mkInstance = function mkInstance(isid, te, did, doc)
{
	let instance = {isid:isid, te:te, did:did, doc:doc};
	instance.__proto__ = this.baseInstance;
	return instance;
}


return manager;

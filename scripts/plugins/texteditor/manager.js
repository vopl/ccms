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
manager.genIsid = function mkTesid(teid, uid)
{
	if(!teid) throw Error("manager.genIsid: teid must be setted");
	if('object' == typeof(teid)) teid = teid.id;
	if(!(teid in this.tes)) throw Error("manager.genIsid: invalud teid");

	if(!uid) uid = user.id;

	let iKey = 
	{
		teid:teid,
		uid:uid,
		iid:crypto.rand.str_(8),
	};
	let pswd = router.cd.global.session.secret;
	let isid = crypto.sym.aes.encodeJson(pswd, iKey);
	return isid;
}

///////////////////////////////////////
manager.getInstance = function getInstance(isid, uid)
{
	if(!uid) uid = user.id;
	let pswd = router.cd.global.session.secret;

	let iKey = crypto.sym.aes.decodeJson(pswd, isid);
	dumpe(iKey);

	if(	!iKey || 
		'object' != typeof(iKey) ||
		iKey.uid != uid ||
		!(iKey.teid in this.tes))
	{
		return undefined;
	}


	let te = this.tes[iKey.teid];

	if(!(iKey.iid in this.instances))
	{
		this.instances[iKey.iid] = this.mkInstance(iKey.iid, te);
	}

	//assert(this.instances[teKey.iid].id == teKey.iid)
	//assert(this.instances[teKey.iid].te.id == teKey.teid)

	return this.instances[iKey.iid];
}

///////////////////////////////////////////
manager.dropInstance = function mkInstance(instance)
{
	if(instance.id in this.instances)
	{
		//assert(this.instances[instance.id].id == instance.id)
		delete this.instances[instance.id];
	}
}

///////////////////////////////////////////
manager.mkInstance = function mkInstance(id, te)
{
	let instance = {id:id, te:te};
	instance.__proto__ = this.baseInstance;
	return instance;
}


return manager;

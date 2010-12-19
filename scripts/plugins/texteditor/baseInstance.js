let baseInstance = 
{
	isid:undefined,
	te:undefined,
	did:undefined,
	doc:undefined,

};

//		onReset:function(did){warn('onReset')},
//		onSave:function(did,doc){warn('onSave')},
//		onFinish:function(did){warn('onFinish')},

///////////////////////////////////////////////////////////////////////////
baseInstance.reset = function reset()
{
	let onReset = this.onReset || this.te.options.onReset;
	if(onReset)
	{
		onReset(this);
	}
}

///////////////////////////////////////////////////////////////////////////
baseInstance.render = function render()
{
	return this.te.render(this.isid, this.doc);
}

///////////////////////////////////////////////////////////////////////////
baseInstance.save = function save(content)
{
	let onSave = this.onSave || this.te.options.onSave;

	if(onSave)
	{
		this.doc = this.te.e2i(content);
		onSave(this);
	}
}

///////////////////////////////////////////////////////////////////////////
baseInstance.setImage = function setImage()
{
	warn('setImage');
}

///////////////////////////////////////////////////////////////////////////
baseInstance.setFile = function setFile()
{
	warn('setFile');
}

///////////////////////////////////////////////////////////////////////////
baseInstance.setContent = function setContent(content)
{
	this.doc = this.te.e2i(content);
}

///////////////////////////////////////////////////////////////////////////
baseInstance.getContent = function getContent()
{
	return this.te.i2e(this.doc);
}

///////////////////////////////////////////////////////////////////////////
baseInstance.getImages = function getImages()
{
	warn('getImages');
}

///////////////////////////////////////////////////////////////////////////
baseInstance.getFiles = function getFiles()
{
	warn('getFiles');
}

///////////////////////////////////////////////////////////////////////////
baseInstance.finish = function finish()
{
	let onFinish = this.onFinish || this.te.options.onFinish;
	if(onFinish)
	{
		onFinish(this);
	}
}

return baseInstance;
(function(){
var Transport = new Function();

///////////////////////////////////////////////////////////
Transport.prototype.makeAjax = function()
{
	if (window.XMLHttpRequest)
	{
		return new XMLHttpRequest();
	} 
	else if (window.ActiveXObject)
	{
		return  new ActiveXObject("Microsoft.XMLHTTP");
	}

	alert('unable to make ajax');
	return null;
}

///////////////////////////////////////////////////////////
Transport.prototype.onReadyState = function(t)
{
	if(!t.responder)
	{
		return;
	}
	var req = t.req;
	try
	{
		switch(req.readyState)
		{
		case 0:// — не инициализирован, 
			if(t.responder.down)t.responder.down(req);
			break;
		case 1:// — открыт, 
			if(t.responder.open)t.responder.open(req);
			break;
		case 2:// — отправка данных, 
			if(t.responder.send)t.responder.send(req);
			break;
		case 3:// — получение данных и 
			if(t.responder.receive)t.responder.receive(req);
			break;
		case 4:// — данные загружены
			switch(req.status)
			{
			case 200:
				if(t.responder.complete)t.responder.complete(req);
				break;
			default:
				if(t.responder.error)t.responder.error(req);
			}
			break;
		}
	}
	catch(e)
	{
		alert('transport.onReadyState: '+e);
	}
}

///////////////////////////////////////////////////////////
Transport.prototype.call = function(uri, data, responder, async)
{
	var t = new Object();
	t.uri = uri;
	t.data = data;
	t.req = this.makeAjax();
	t.responder = responder;

	var params = null;
	if(data)
	{
		params = '';
		for(var key in data)
		{
			if('object' == typeof(data[key]))
			{
				for(key2 in data[key])
				{
					if('' != params)
					{
						params += '&';
					} 
					params += encodeURIComponent(key) + "=" + encodeURIComponent(data[key][key2]);
				}
			}
			else
			{
				if('' != params)
				{
					params += '&';
				} 
				params += encodeURIComponent(key) + "=" + encodeURIComponent(data[key]);
			}
		}
		//alert(armedUrl);
	}
	
	var method = 'GET';
	if(params)
	{
		method = 'POST';
	}
	t.req.open(method, uri, async);
	if(params)
	{
		t.req.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
	}
	
	if(async)
	{
		var t2=this;
		t.req.onreadystatechange = function()
		{
			return t2.onReadyState(t);
		};
	}
	
	t.req.send(params);
	if(!async)
	{
		this.onReadyState(t);
	}
}


///////////////////////////////////////
Transport.prototype.scriptMap = new Object;

///////////////////////////////////////
Transport.prototype.loadScript=function(urljs, readyCallback, readyCallbackArgs)
{
	this.scriptMap[urljs] = 1;
	
	var thisObj = this;
	var responder = 
	{
		complete:function(req)
		{
			thisObj.scriptMap[urljs]=2;

			try
			{
				eval(req.responseText);
				if(readyCallback)
				{
					readyCallback(readyCallbackArgs);
				}
			}
			catch(e)
			{
				alert('Transport.loadScript: '+e+", "+urljs);
			}
		},
		error:function(){alert('error loading '+urljs)},
	}
	
	this.call(urljs, null, responder);
}

///////////////////////////////////////
Transport.prototype.loadScriptOnce=function(urljs, readyCallback, readyCallbackArgs)
{
	if(!this.scriptMap[urljs])
	{
		this.loadScript(urljs, readyCallback, readyCallbackArgs);
	}
	else if(1 == this.scriptMap[urljs])
	{
		this.waitScriptLoad(urljs, readyCallback, readyCallbackArgs);
	}
	else if(2 == this.scriptMap[urljs])
	{
		if(readyCallback)
		{
			readyCallback(readyCallbackArgs);
		}
	}
}

//////////////////////////////////////
Transport.prototype.waitScriptLoad=function(urljs, readyCallback, readyCallbackArgs)
{
	if(1 == this.scriptMap[urljs])
	{
		var thisObj = this;
		setTimeout((function(){thisObj.waitScriptLoad(urljs, readyCallback, readyCallbackArgs);}), 100 );
	}
	else if(2 == this.scriptMap[urljs])
	{
		readyCallback(readyCallbackArgs);
	}
}

window.transport = new Transport();
})()
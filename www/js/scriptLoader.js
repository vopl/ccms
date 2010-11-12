function ScriptLoader(mark)
{
	this.mark = document.getElementById(mark);
	if(!this.mark)
	{
		if(window.XULDocument && document instanceof window.XULDocument)
		{
			this.mark = document.createElement('box');
			this.mark.style.display = 'none';
			this.mark.id = mark;
			document.documentElement.appendChild(this.mark);
		}
		else
		{
			this.mark = document.createElement('span');
			this.mark.style.display = 'none';
			this.mark.id = mark;
			document.documentElement.appendChild(this.mark);
			//alert('what kind of document?');
		}
		//alert(document);
	}
}

ScriptLoader.prototype.map = new Object;

///////////////////////////////////////
ScriptLoader.prototype.load=function(urljs, readyCallback, readyCallbackArgs)
{
	this.map[urljs] = 1;
	var thisObj = this;
	var script = document.createElement("script");
	script.setAttribute('type', 'text/javascript');
	script.onload = function()
	{
		thisObj.map[urljs]=2;
		if(readyCallback)
		{
			readyCallback(readyCallbackArgs);
		}
	};
	
	if(this.mark.nextSibling)
	{
		this.mark.parentNode.insertBefore(script, this.mark.nextSibling);
	}
	else
	{
		this.mark.parentNode.appendChild(script);
	}

	script.setAttribute('src', urljs);
}

///////////////////////////////////////
ScriptLoader.prototype.loadOnce=function(urljs, readyCallback, readyCallbackArgs)
{
	if(!this.map[urljs])
	{
		this.load(urljs, readyCallback, readyCallbackArgs);
	}
	else if(1 == this.map[urljs])
	{
		this.waitLoad(urljs, readyCallback, readyCallbackArgs);
	}
	else if(2 == map[urljs])
	{
		readyCallback(readyCallbackArgs);
	}
}

//////////////////////////////////////
ScriptLoader.prototype.waitLoad=function(urljs, readyCallback, readyCallbackArgs)
{
	if(1 == this.map[urljs])
	{
		var thisObj = this;
		setTimeout((function(){thisObj.waitLoad(urljs, readyCallback, readyCallbackArgs);}), 100 );
	}
	else if(2 == this.map[urljs])
	{
		readyCallback(readyCallbackArgs);
	}
}

var sl = new ScriptLoader('scriptLoader');

//mode = replace|insertBefore|replaceChilds|appendChild
window.DomReplacer = function (id, callback, mode)
{
	this.id = id;
	this.callback = callback;
	this.mode = mode?mode:'replace';
}
//Replacer.prototype.open=function(req){alert("open");}
//Replacer.prototype.send=function(req){alert("send");}
//Replacer.prototype.receive=function(req){alert("receive");}

window.DomReplacer.prototype.complete=function(req)
{
	var newElement = req.responseXML.documentElement;
	if(!newElement)
	{
		var string = (new XMLSerializer()).serializeToString(req.responseXML);
		alert("error in xml:\n"+string);
	}
	this.trimWhite(newElement);

	newElement = document.adoptNode(newElement);
	var oldElement = document.getElementById(this.id);
	
	switch(this.mode)
	{
	case 'insertBefore':
		oldElement.parentNode.insertBefore(newElement, oldElement);
		break;
	case 'replace':
		oldElement.parentNode.replaceChild(newElement, oldElement);
		break;
	case 'replaceChilds':
		while(oldElement.firstChild) oldElement.removeChild(oldElement.firstChild);
		oldElement.appendChild(newElement);
		break;
	case 'appendChild':
		oldElement.appendChild(newElement);
		break;
	}
	
	this.evalScripts(newElement);
	if(this.callback)
	{
		this.callback();
	}
}

//Replacer.prototype.error=function(req){alert("error");}

window.DomReplacer.prototype.evalScripts = function(element)
{
	if(!element)
	{
		return;
	}
	if(element.tagName && element.tagName.toLowerCase()=='script')
	{
		try
		{
			eval(element.textContent);
		}
		catch(e)
		{
			alert('window.DomReplacer.evalScripts: '+e+", "+element.textContent);
		}
	}
	else
	{
		for(var chIdx in element.childNodes)
		{
			this.evalScripts(element.childNodes[chIdx])
		}
	}
	return;
}

window.DomReplacer.prototype.trimWhite = function(element)
{
	for(var chIdx =0; chIdx<element.childNodes.length;)
	{
		var c = element.childNodes[chIdx];
		if(c.nodeName == '#text')
		{
			if(c.textContent.match(/^(\s|\r|\n)*$/))
			{
				element.removeChild(c);
				continue;
			}
		}
		this.trimWhite(c);
		chIdx++;
	}
	return;
}

//window.DomReplacer = DomReplacer;

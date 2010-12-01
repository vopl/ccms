let pathPart = arguments[0];
if(pathPart in this.childs)
{
	return {point:this.childs[pathPart]};
}
return undefined;
let options = arguments[0];
if(user.login != 'admin' || !options.childs)
{
	return null;
}

let res={
	point:this,
	childs:{},
};

for(let cname in this.childs)
{
	res.childs[cname] = this.childs[cname].map(options);
}

return res;
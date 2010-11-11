var plug=
{
	name:'nsTree',
	
	installImpl:function()
	{
		exec("schema.js", orm, true);
		return true;
	},
	uninstallImpl:function()
	{
		exec("schema.js", orm, false);
		return true;
	},


	deserializeImpl:function()
	{
		exec("schema.js", orm, true);
		return true;
	},
	


	
};
plug.__proto__ = exec(this, "../basePlugin.js");
return plug;
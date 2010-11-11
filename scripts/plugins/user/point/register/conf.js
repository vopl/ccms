var res=
{
	properties:
	{
		process:{file:'./process.js'},
		render:{file:'./render.js'},

		registerConfirmation:{file:'./registerConfirmation.js'},
	},

	childs:
	{
		confirm:'confirm/conf.js',
		resendConfirm:'resendConfirm/conf.js'
	}
};
return res;
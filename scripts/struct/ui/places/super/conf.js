let res = {
	properties:
	{
		placeName:{value:'warn("placeName must be setted for "+this.path);'},

		render:{file:'./render.js'},

		pre:{value:'',executable:false},

		data:{file:'data.js'},
		wrapperElement:{file:'wrapperElement.js'},
		wrapperArray:{file:'wrapperArray.js'},

		post:{value:'',executable:false},
	},
};
return res;
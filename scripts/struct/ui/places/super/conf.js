let res = {
	properties:
	{
		placeName:{value:'warn("placeName must be setted for "+this.path);'},

		render:{file:'./render.js'},

		pre:{value:'',executable:false},
		wrapper:{file:'wrapper.js'},
		data:{file:'data.js'},
		post:{value:'',executable:false},
	},
};
return res;
let obj=
{
	path:request.path,

	paramsGet:request.paramsGet,

	paramsPost:request.paramsPost,
	files:request.files,

	params:request.params,

	cookies:request.cookies,

	env:request.env,

};
dump(obj, 5);
let target = request.planData.forum;

let point = this;














return <>

{(function(){
	let res = <></>;

	for each(let forum in target.childs)
	{
		res += point.properties.renderNavigateRow(forum);
	}
	return <ul>{res}</ul>;
})()}

	

</>


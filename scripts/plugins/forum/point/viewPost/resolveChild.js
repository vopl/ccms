let pathPart = arguments[0];
let data = arguments[1];


let mostPost = data.post;

//////////////////////////////////////////////////
if(pathPart == 'edit')
{
	return {
		title:_('edit post $name', {$name:mostPost.map_title}),
		point:this.parent.childs.editPost,
	};
}

//////////////////////////////////////////////////
if(pathPart == 'del')
{
	return {
		point:this.parent.childs.delPost
	};
}

//////////////////////////////////////////////////
if(pathPart == 'add')
{
	data.mode = 'add';
	return {
		title:_('add post'),
		point:this.parent.childs.editPost,
	};
}




//дочерний
let post;
//попробовать дочерний по id
if(Math.round(pathPart)==pathPart) post = mostPost.childsById[pathPart];
//попробовать дочерний по map_path
if(!post) post = mostPost.childsByPath[pathPart];




if(post)
{
	//дочерний найден
	data.post = post;

	return {
		title:post.title,
		point:this,
	};
}



warn('unknown path for '+absFile()+", '"+pathPart+"'");
return false;
let teid = request.params.teid;
if(!teid)
{
	request.setStatusCode(403);//Forbidden
	return;
}

let sid = router.cd.global.session.id;
let uid = global.user.id;

//let manager = {};
let instance = router.plugins.texteditor.manager.getInstance(teid, sid, uid);

if(!instance)
{
	request.setStatusCode(404);//Not Found
	return;
}

let res;

switch(request.params.mode)
{
case 'setImage':
	res = instance.setImage();
	break;
case 'setFile':
	res = instance.setFile();
	break;
case 'setContent':
	res = instance.setContent();
	break;
case 'reset':
	res = instance.reset();
	break;
case 'getContent':
	res = instance.getContent();
	break;
case 'getImages':
	res = instance.getImages();
	break;
case 'getFiles':
	res = instance.getFiles();
	break;
case 'drop':
	res = manager.drop(instance);
	break;
default:
	request.setStatusCode(501);//Not Implemented
	return;
}


//создать ресурс
//удалить ресурс
//почистить ресурс по таймауту

//вычислить разрешения
/*
	salt+sid+uid -> разрешения
	происходят из инициирующего контроллера
*/

//добавить/удалить/обновить картинку
//добавить/удалить/обновить файл
//обновить контент

this.render(request.params.mode, res);

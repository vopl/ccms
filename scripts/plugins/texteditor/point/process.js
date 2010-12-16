
let isid = request.params.isid;
if(!isid)
{
	request.setStatusCode(403);//Forbidden
	return;
}

let manager = router.plugins.texteditor.manager;
let instance = manager.getInstance(isid);

if(!instance)
{
	request.setStatusCode(404);//Not Found
	return;
}

let res;

switch(request.params.mode)
{
case 'render':
	res = instance.render().print();
	break;
case 'ping':
	res = manager.pingInstance(instance);
	break;
case 'save':
	res = instance.save(request.params.content);
	break;
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
	instance.finish();
	res = manager.dropInstance(instance);
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
	salt+sid -> разрешения
	происходят из инициирующего контроллера
*/

//добавить/удалить/обновить картинку
//добавить/удалить/обновить файл
//обновить контент

this.render(request.params.mode, res);

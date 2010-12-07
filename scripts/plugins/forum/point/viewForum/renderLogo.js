let target = request.planData.forums.back;
let point = this;

let img = orm.Image.load({id:target.image_id});

if(!img) return "";
let t = arguments.callee.t;
if(!t)
{
	t = router.createTemplate();
	t.compile(<img src={t.src} width={t.width} height={t.height}/>);
	arguments.callee.t = t;
}
t = t.clone();

t.src = 'filestore/'+img.location_thumb;
t.width = 'filestore/'+img.width_thumb;
t.height = 'filestore/'+img.height_thumb;

return t;

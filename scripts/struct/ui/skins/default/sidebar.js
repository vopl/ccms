let data = [];

if(arguments.length>0)
	data.push(ui.skin.divclass(arguments[0], 'sidebar-content'));

if(arguments.length>1)
	data.unshift(ui.skin.divclass(arguments[1], 'sidebar-header'));
	
if(arguments.length>2)
	data.push(ui.skin.divclass(arguments[2], 'sidebar-footer'));

return ui.skin.divclass(data, 'sidebar');

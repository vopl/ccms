// let data = [];

// if(arguments.length>0)
	// data.push(ui.skin.tag('div', arguments[0], {class:'sidebar-content'}));
// //	data.push(ui.skin.divclass(arguments[0], 'sidebar-content'));

	
	
	
	
// if(arguments.length>1)
	// data.unshift(ui.skin.divclass(arguments[1], 'sidebar-header'));
	
// if(arguments.length>2)
	// data.push(ui.skin.divclass(arguments[2], 'sidebar-footer'));

// return ui.skin.divclass(data, 'sidebar');




let data = [];

if(arguments.length>0)
	data.push(ui.skin.tag('div', arguments[0], {class:'sidebar-content'}));

if(arguments.length>1)
	data.unshift(ui.skin.tag('div', arguments[1], {class:'sidebar-header'}));

if(arguments.length>2)
	data.push(ui.skin.tag('div', arguments[2], {class:'sidebar-footer'}));

return ui.skin.tag('div', data, {class:'sidebar'});

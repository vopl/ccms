﻿let point = this;

if(request.params.mode && request.params.mode=='update')
{
	switch(request.params.mode)
	{
	case 'update':
		let revision = Number(request.params.revision || 0);
		let id = Number(request.params.id || 0);
		
		let res = [];

		function rrender(post)
		{
			if(post.revision > revision || post.id==id)
			{
				res.push(point.properties.renderRow(post));
			}
			else
			{
				for each(let c in post.childs)
				{
					rrender(c);
				}
			}
		}

		rrender(request.planData.post, res);
		

		request.pushHeader('Content-Type', "text/xml; charset=utf-8");
		print('<?xml version="1.0" encoding="utf-8"?>\n');
		print('<update>');
			print('<post>');
				res.forEach(function(el) el.print());
			print('</post>');
		print('</update>');
		return;
	default:
		break;
	}
}

ui.blocks.center.push(
	acl.hasRight('forum', 'viewPost')?
		this.render():
		ui.skin.accessDenied());
ui.print();




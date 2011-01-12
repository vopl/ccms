	if(!window.ccms) window.ccms = {};

	window.ccms.postTreeManager = 
	{
		updateUrl:undefined,
		addUrl:undefined,
		editUrl:undefined,
		
		defaultOptions:
		{
			aproperty:'marginLeft',
			apropertyMin:'0.5',
			apropertyMax:'2',
			apropertyUnit:'em',
			aduration:200,
			triggerInterval:200,
			hmode:'auto',
		},


		lastHMode:null,
		lastTriggerTime:new Date(),
		lastTopElement:null,
		lastBottomElement:null,
		lastlevel:-1,
		zeroLevel:0,
		revision:0,

		init:function(updateUrl, options)
		{
			var man = this;

			if(!updateUrl) updateUrl = window.location.href;
			man.updateUrl = updateUrl;
			man.addUrl = updateUrl+'/add';
			man.editUrl = updateUrl+'/edit';
			
			if(!options) options = {};
			options.__proto__ = man.defaultOptions;
			man.__proto__ = options;


			man.lastTopElement = null;
			man.lastBottomElement = null;
			man.lastlevel = -1;
			
			man.zeroLevel = 1e10;
			$('.forum-post-structor').each(function(idx, el){

				var l = Number(el.getAttribute('level'));
				if(man.zeroLevel>l)
				{
					man.zeroLevel = l;
				}
			});

			man.evalLastRevision();
			
			$('#forum-tree-man-h'+man.hmode).attr('checked', true);

			man.trigger(true);
			$(document).ready(function(){man.onViewChanged(true)});
			$(window).scroll(function(){man.onViewChanged()});
		},
		
		evalLastRevision:function()
		{
			var man = this;
			
			man.revision = 0;
			var kk = $('.forum-post');
			$('.forum-post').each(function(idx, el){

				var l = Number(el.getAttribute('rev'));
				if(man.revision<l)
				{
					man.revision = l;
				}
			});
		},

		onViewChanged:function(force)
		{
			var man = this;

			var delta = (new Date()).getTime() - man.lastTriggerTime.getTime();
			if(delta >= man.triggerInterval) man.trigger(force);
			else setTimeout(function(){window.ccms.postTreeManager.trigger(force)}, man.triggerInterval-delta);
		},

		trigger:function(force)
		{
			var man = this;

			man.lastTriggerTime = new Date();

			switch(man.hmode)
			{
			case 'min':
				man.lastHMode = man.hmode;
				man.lastTopElement = null;
				man.lastBottomElement = null;
				man.lastLevel = 1e10;
				man.applyLevel(1e10);
				return;
			case 'max':
				man.lastHMode = man.hmode;
				man.lastTopElement = null;
				man.lastBottomElement = null;
				man.lastLevel = 0;
				man.applyLevel(0);
				return;
			}

			var top = window.scrollY;
			var bottom = top + $(window).height();
			var bottom2 = top + Math.floor($(window).height()*1.5);

			var min = 1e10;
			var minEl;
			var level = 1e10;
			var max = 0;
			var maxEl;
			
			var elements = [];
			$('.forum-post-structor').each(function(idx, el){

				var t = $(el).offset().top;
				var b = t + el.clientHeight;
				if(b>=top && t<=bottom)
				{
					elements.push(el);

					var l = Number(el.getAttribute('level'));
					if(level>l)
					{
						level = l;
					}

					if(max<t)
					{
						max=t;
						maxEl = el;
					}
					if(min>t)
					{
						min=t;
						minEl = el;
					}
				}
				else if(b>=top && t<=bottom2)
				{
					elements.push(el);
				}
				else
				{
					//out of window
				}
			});

			if(level>=1e10) level = 0;

			if(	man.lastHMode != man.hmode ||
				man.lastTopElement != minEl ||
				man.lastBottomElement != maxEl ||
				man.lastLevel != level ||
				force)
			{
				man.lastHMode = man.hmode;
				man.lastTopElement = minEl;
				man.lastBottomElement = maxEl;
				man.lastLevel = level;
				man.applyLevel(level, elements);
			}
		},

		applyLevel:function(level, elements)
		{
			var man = this;
			if(!elements) elements = $('.forum-post-structor');

			level -= man.zeroLevel;
			for(var idx=0; idx<elements.length; idx++)
			{
				el = $(elements[idx]);

				var cl = Number(el.attr('level')) - man.zeroLevel;

				//el.clearQueue();
				el.stop();

				var c = {}; 
				if(cl < level)	c[man.aproperty] = '0'+man.apropertyUnit;
				else		c[man.aproperty] = String(man.apropertyMax*(cl-level) + man.apropertyMin*(level))+man.apropertyUnit;

				el.animate(c, man.aduration, 'swing');
			};
		},


		collapse:function(mode, elements)
		{
			var man = this;
			if(!elements) elements = $('.forum-post-content');

			if(mode) elements.hide(man.aduration, function(){man.onViewChanged()});
			else elements.show(man.aduration, function(){man.onViewChanged()});
		},


		///////////////////////////////////
		set:function(mode)
		{
			var man = this;

			switch(mode)
			{
			case 'hmin':
				man.hmode = 'min';
				//man.trigger(true);
				man.onViewChanged(true);
				break;
			case 'hauto':
				man.hmode = 'auto';
				//man.trigger(true);
				man.onViewChanged(true);
				break;
			case 'hmax':
				man.hmode = 'max';
				//man.trigger(true);
				man.onViewChanged(true);
				break;
			case 'vmin':
				man.collapse(true);
				break;
			case 'vmax':
				man.collapse(false);
				break;
			}
		},




		///////////////////////////////////
		update:function(id)
		{
			var man = this;
			$.ajax({
				url: man.updateUrl, 
				data: {mode:'update', revision:man.revision, id:id},
				//data: {mode:'update', revision:0},
				dataType: 'xml',
				context: man, 
				success: man.updateSuccess,
			});
		},

		updateSuccess:function(data, textStatus, req)
		{
			var man = this;

			var posts = $(data).find( "post").children();
			if(posts.length)
			{
				$('.forum-post-new').removeClass('forum-post-new');
				posts.each(function(idx, el)
				{
					var old = $('#'+$(el).attr('id'));
					
					if(old.length)
					{
						//replace
						old.after((new XMLSerializer()).serializeToString(el)).remove();
					}
					else
					{
						//insert
						var parent = $('#forum-post-'+$(el).attr('pid') + ' > .forum-post-childs');
						if(parent.length)
						{
							parent.append((new XMLSerializer()).serializeToString(el));
						}
						else
						{
							alert('page corrupted');
							
							parent = $('#forum-posts');
							if(parent.length)
							{
								parent.append((new XMLSerializer()).serializeToString(el));
							}
							else
							{
								alert('page corrupted absolutely');
							}
						}
					}

					$('#'+$(el).attr('id')).addClass('forum-post-new');
				});
				man.evalLastRevision();
				man.onViewChanged(true);
			}
			else
			{
				//nothing to update
			}
		},
		
		
		
		
		
		
		
		
		
		addAnswerForm: function(id, pid)
		{
			var man = this;

			var area = $('#forum-post-'+(id||pid) + ' >.forum-post-structor > .forum-post-answer');
			if(area.children().length)
			{
				area.empty();
			}
			else
			{
				//area.append($('<b>sdfg</b>'));
				
				$.ajax({
					url: id?man.editUrl:man.addUrl, 
					data: {mode:'answerForm', id:id, pid:pid},
					dataType: 'xml',
					success: function(data, textStatus, req) {man.injectAnswerFormSuccess(id, pid, data, textStatus, req);},
				});
				
			}
		},
		
		injectAnswerFormSuccess: function(id, pid, data, textStatus, req)
		{
			var man = this;
			
			var area = $('#forum-post-'+(id||pid) + ' >.forum-post-structor > .forum-post-answer');
			var form = $(data).find( "form").get(0);
			form = (new XMLSerializer()).serializeToString(form);
			area.append(form);
		},
		
		doAnswer: function(id, pid, send)
		{
			var man = this;

			if(send)
			{
				var form = $('#forum-post-'+(id||pid) + ' >.forum-post-structor > .forum-post-answer > form');
				form.ajaxSubmit({
					url: id?man.editUrl:man.addUrl, 
					data: {mode:'answer', id:id, pid:pid},
					dataType: 'xml',
					success: function(data, textStatus, req) {man.answerFormSuccess(id, pid, data, textStatus, req);},
				});
			}
			else
			{
				$('#forum-post-'+(id||pid) + ' >.forum-post-structor > .forum-post-answer').empty();
			}
		},
		
		answerFormSuccess: function(id, pid, data, textStatus, req)
		{
			var man = this;

			$('#forum-post-'+(id||pid) + ' >.forum-post-structor > .forum-post-answer').empty();
			man.update(id);
		}

	};

	if(!window.ccms) window.ccms = {};

	window.ccms.postTreeManager = 
	{
		updateUrl:undefined,
		
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

		init:function(updateUrl, options)
		{
			var man = this;

			if(!updateUrl) updateUrl = window.location.href;
			man.updateUrl = updateUrl;

			if(!options) options = {};
			options.__proto__ = man.defaultOptions;
			man.__proto__ = options;


			man.lastTopElement = null;
			man.lastBottomElement = null;
			man.lastlevel = -1;

			$('#forum-tree-man-h'+man.hmode).attr('checked', true);

			man.trigger(true);
			$(document).ready(function(){man.onViewChanged(true)});
			$(window).scroll(function(){man.onViewChanged()});
		},

		onViewChanged:function(force)
		{
			var man = this;

			var delta = (new Date()).getTime() - man.lastTriggerTime.getTime();
			if(delta >= man.triggerInterval) man.trigger();
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
			var bottom2 = top + Math.floor($(window).height()*1.1);

		        var min = 1e10;
		        var minEl;
		        var level = 1e10;
		        var max = 0;
		        var maxEl;

		        var elements = [];
			$('.forum-post-structor').each(function(idx, el){
				var t = $(el).offset().top;
				var b = $(el).offset().top+el.clientHeight;
				if(b>=top && t<=bottom)
				{
					elements.push(el);

					var l = el.getAttribute('level');
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

			for(var idx=0; idx<elements.length; idx++)
			{
				el = $(elements[idx]);

				var cl = Number(el.attr('level'));

				//el.clearQueue();
				el.stop();

				var c = {}; 
				if(level > cl)	c[man.aproperty] = String(man.apropertyMin*(cl))+man.apropertyUnit;
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
		update:function()
		{
			var man = this;
			$.ajax({
				url: man.updateUrl, 
				data: {mode:'update'},
				dataType: 'xml',
				context: man, 
				success: man.updateSuccess,
			});
		},

		updateSuccess:function(data, textStatus, req)
		{
			var man = this;
			alert(data);
		},

	};

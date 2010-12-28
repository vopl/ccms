	if(!window.ccms) window.ccms = {};

	window.ccms.postTreeManager = 
	{
		aproperty:'marginLeft',
		apropertyMin:'0.5',
		apropertyMax:'2',
		apropertyUnit:'em',
		aduration:200,
		triggerInterval:0,
		lastTriggerTime:new Date(),
		lastTopElement:null,
		lastBottomElement:null,
		lastlevel:-1,

		init:function(aproperty, apropertyMin, apropertyMax, apropertyUnit, aduration, triggerInterval)
		{
			var man = this;

			if(undefined !== aproperty) man.aproperty = aproperty;
			if(undefined !== apropertyMin) man.apropertyMin = apropertyMin;
			if(undefined !== apropertyMax) man.apropertyMax = apropertyMax;
			if(undefined !== apropertyUnit) man.apropertyUnit = apropertyUnit;
			if(undefined !== aduration) man.aduration = aduration;
			if(undefined !== triggerInterval) man.triggerInterval = triggerInterval;

			man.lastTopElement = null;
			man.lastBottomElement = null;
			man.lastlevel = -1;

			man.trigger();
			$(window).scroll(function(){man.onScroll()});
		},

		onScroll:function()
		{
			var man = this;

			var delta = (new Date()).getTime() - man.lastTriggerTime.getTime();
			if(delta >= man.triggerInterval) man.trigger();
			else setTimeout(function(){window.ccms.postTreeManager.trigger()}, man.triggerInterval-delta);
		},

		trigger:function()
		{
			var man = this;

			man.lastTriggerTime = new Date();
			
			var top = window.scrollY;
			var bottom = top + $(window).height();
		
		        var min = 1e10;
		        var minEl;
		        var level = 1e10;
		        var max = 0;
		        var maxEl;
			$('.forum-post-tree-structor').each(function(idx, el){
				var t = $(el).offset().top;
				var b = $(el).offset().top+el.clientHeight;
				if(b>=top && t<=bottom)
				{
					el.setAttribute('tree-state', 'in');

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
				else
				{
					el.setAttribute('tree-state', 'out');
				}
			});

			if(level>=1e10) level = 0;

			if(	man.lastTopElement != minEl ||
				man.lastBottomElement != maxEl ||
				man.lastLevel != level)
			{
				man.lastTopElement = minEl;
				man.lastBottomElement = maxEl;
				man.lastLevel = level;
				man.applyLevel(level);
			}
		},

		applyLevel:function(level)
		{
			var man = this;

			window.status = level;
			$('.forum-post-tree-structor').each(function(edx, el){
				var cl = Number(el.getAttribute('level'));
				var state = el.getAttribute('tree-state');

				el = $(el);

				var c = {}; 
				
				if(level > cl)	c[man.aproperty] = String(man.apropertyMin*(cl))+man.apropertyUnit;
				else		c[man.aproperty] = String(man.apropertyMax*(cl-level) + man.apropertyMin*(level))+man.apropertyUnit;

				if(state == 'in')
				{
					el.clearQueue();
					el.stop();
					el.animate(c, man.aduration, 'swing');
				}
				else
				{
					//ignore
				}


			});
		}
	};

	if(!window.ccms) window.ccms = {};

	window.ccms.postTreeManager = 
	{
		aproperty:'paddingLeft',
		apropertyMin:'0.5em',
		apropertyMax:'3em',
		aduration:200,
		triggerInterval:500,
		lastTriggerTime:new Date(),
		lastLevel:-2,

		init:function(aproperty, apropertyMin, apropertyMax, aduration, triggerInterval)
		{
			var man = this;

			if(undefined !== aproperty) man.aproperty = aproperty;
			if(undefined !== apropertyMin) man.apropertyMin = apropertyMin;
			if(undefined !== apropertyMax) man.apropertyMax = apropertyMax;
			if(undefined !== aduration) man.aduration = aduration;
			if(undefined !== triggerInterval) man.triggerInterval = triggerInterval;

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
		
			var max = 0;
			var maxEl = null;
			$('.forum-post-tree-structor').each(function(edx, el){
				var t = el.offsetTop;
				if(t<top && t>max)
				{
					max = t;
					maxEl = el;
				}
			});

			var level = maxEl?Number(maxEl.getAttribute('level')):-1;
			if(level != man.lastLevel)
			{
				man.applyLevel(level);
				man.lastLevel = level;
			}
		},

		applyLevel:function(level)
		{
			var man = this;

			window.status = level;
			$('.forum-post-tree-structor').each(function(edx, el){
				var cl = Number(el.getAttribute('level'));
				if(cl <= level)
				{
					var c = {}; c[man.aproperty] = man.apropertyMin;
					$(el).animate(c, man.aduration);
				}
				else
				{
					var c = {}; c[man.aproperty] = man.apropertyMax;
					$(el).animate(c, man.aduration);
				}
			});
		}
	};

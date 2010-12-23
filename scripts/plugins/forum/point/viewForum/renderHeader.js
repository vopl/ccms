﻿let target = arguments[0];if(!target) target = request.planData.forum;let point = this;let t = arguments.callee.t;if(!t){	t = router.createTemplate();	t.compile(<div>		<div style='float: left;'>			{t.logo}		</div>		<div style='float: right;'>			{t.manage}		</div>		<div>			<strong>{t.title}</strong>		</div>				<div>			тем {t.topicsInf},			за неделю {t.topics168},			за сутки {t.topics24}:			ответов {t.postsInf},			за неделю {t.posts168},			за сутки {t.posts24}		</div>				{t.addTopicLink}					<div style='clear: both'/>		<div>			новые {t.newTopics} <br/>			последние {t.lastTopics} <br/>			страницы {t.pages}<br/>		</div>	</div>);	arguments.callee.t = t;}t = t.clone();//сформировать картинкуif(target.image_id){	let img = global.cache.process({		key:'forum.'+target.id+'.image',		provider:function() orm.Image.load({id:target.image_id}),		events:['forum.forum', 'forum.'+target.id],	});	t.logo = ui.skin.tag(		'img', 		undefined,		{			src:'/filestore/'+img.location_thumb, 			width:img.width_thumb,			height:img.height_thumb,		}	);}t.manage = this.properties.renderManage(target);t.title = target.title;//сформировать статистикуlet statTopics = global.cache.process({	key:'forum.'+target.id+'.statTopics',	provider:function()	{		let res = {};		res.inf = orm.query('SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1', target.id)[0].count;		res._24 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 AND ctime>(now()-interval '24 hours')", target.id)[0].count;		res._168 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 AND ctime>(now()-interval '168 hours')", target.id)[0].count;		return res;	},	events:['forum.forum', 'forum.'+target.id+'.topic'],});let statPosts = global.cache.process({	key:'forum.'+target.id+'.statPosts',	provider:function()	{		let res = {};		res.inf = orm.query('SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NOT NULL AND forum_id=$1', target.id)[0].count;		res._24 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NOT NULL AND forum_id=$1 AND ctime>(now()-interval '24 hours')", target.id)[0].count;		res._168 = orm.query("SELECT COUNT(*) FROM {ForumPost} WHERE tree_pid IS NOT NULL AND forum_id=$1 AND ctime>(now()-interval '168 hours')", target.id)[0].count;		return res;	},	events:['forum.forum', 'forum.'+target.id+'.post'],});t.topicsInf = statTopics.inf;t.topics168 = statTopics._168;t.topics24 = statTopics._24;t.postsInf = statPosts.inf;t.posts168 = statPosts._168;t.posts24 = statPosts._24;//сформировать N новых темlet newTopicsAmount = 4;//количество выводимых новых темlet newTopics = global.cache.process({	key:'forum.'+target.id+'.newTopics',	provider:function()	{		let lastVisitTime = new Date(1970, 01, 01);//временная метка, после которой тема считается новой, это надо каждому пользователю ввести метку и устанавливать ее когда пользователь смотрит темы		let res = orm.query("SELECT * FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 AND ctime>$2 ORDER BY ctime DESC LIMIT $3", target.id, lastVisitTime, newTopicsAmount);		res.forEach(function(v)		{			v.title = v.map_title?v.map_title:'topic #' + v.id;			v.pathPart = v.map_path?v.map_path:v.id;			v.path = target.path + '/' + v.pathPart;						v.user = orm.query("SELECT * FROM {User} WHERE id=$1", v.user_id)[0];		});		return res;	},	events:['forum.forum', 'forum.'+target.id+'.topic'],});//сформировать N последних темlet lastTopicsAmount = 4;//количество выводимых последних темlet lastTopics = global.cache.process({	key:'forum.'+target.id+'.lastTopics',	provider:function()	{		let res = orm.query("SELECT * FROM {ForumPost} WHERE tree_pid IS NULL AND forum_id=$1 ORDER BY ctime DESC LIMIT $2", target.id, lastTopicsAmount + newTopicsAmount);				let newIds = {}; newTopics.forEach(function(v)newIds[v.id]=true);		res = res.filter(function(v) !newIds[v.id]);		while(res.length > lastTopicsAmount) res.pop();				res.forEach(function(v)		{			v.title = v.map_title?v.map_title:'topic #' + v.id;			v.pathPart = v.map_path?v.map_path:v.id;			v.path = target.path + '/' + v.pathPart;						v.user = orm.query("SELECT * FROM {User} WHERE id=$1", v.user_id)[0];		});		return res;	},	events:['forum.forum', 'forum.'+target.id+'.topic'],});t.newTopics = newTopics.map(function(v) point.properties.renderTopicPreview(v)).dilute('; ');t.lastTopics = lastTopics.map(function(v) point.properties.renderTopicPreview(v)).dilute('; ');//////////////////////////////////////if(target.topic_allow){	let pages = this.properties.mkTopicPages();	if(pages.rpagesAmount)	{		t.pages = [];		let rpagesAmountBegin = pages.rpagesAmount;		if(rpagesAmountBegin > 5) rpagesAmountBegin = 5;		for(let p=1; p<=rpagesAmountBegin; p++)		{			t.pages.push(ui.skin.tag('a', p, {href:target.path+'/rpage'+p}));		}				let rpagesAmountEnd = pages.rpagesAmount - rpagesAmountBegin;		if(rpagesAmountEnd > 5) rpagesAmountEnd = 5;				if(rpagesAmountEnd>0)		{			t.pages.push('...');			for(let p=pages.rpagesAmount-rpagesAmountEnd; p<=pages.rpagesAmount; p++)			{				t.pages.push(ui.skin.tag('a', p, {href:target.path+'/rpage'+p}));			}		}		t.pages = t.pages.dilute(' ');	}	else if(pages.pages)	{		t.pages = 'для постоянных страниц';		//pages.pages.map(function(v) ui.skin.tag('a', p, {href:target.path+'/page'+p}));	}	else if(pages.dates)	{		t.pages = 'для дат';		//pages.dates.map(function(v) ui.skin.tag('a', v.tsd, {href:target.path+'/'+v.tsd}));	}	t.addTopicLink = ui.skin.tag('a', 'Добавить тему', {href:target.path+'/addTopic?backUrl='+target.path+"/"+(new Date()).tsd});}return t;
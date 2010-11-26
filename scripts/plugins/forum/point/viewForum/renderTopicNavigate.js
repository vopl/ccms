let target = request.planData.forums.back;
let forumPath = request.planData.nonPagePath?request.planData.nonPagePath:request.path;
let page = request.planData.page?request.planData.page:1;

let limit=20;
let offset = (page-1)*limit;
let topics = global.cache.process({
		key:'forum.topic.'+target.id+'.'+limit+'.'+offset,
		provider:function()
		{
			return orm.query('SELECT * FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL ORDER BY id DESC LIMIT $2 OFFSET $3', target.id, limit, offset);
		},
		events:["forum.topic"],
});


let topicsXml = <>навигация по темам</>;
if(target.topics_navigate_date)
{
	topicsXml+=<><br/>по датам за последние 10 дней</>;
	
	let dbr = orm.query('SELECT DISTINCT ctime::DATE FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL ORDER BY ctime DESC LIMIT 10', target.id);
	if(!dbr.length) dbr = [{ctime:new Date()}];
	let lastMonth = -1;
	let lastYear = (new Date()).getFullYear();
	for each(let r in dbr)
	{
		let d = r.ctime;
		let href = target.path+'/'+d.tsd;
		
		let name = [];
		if(lastYear != d.getFullYear())
		{
			name.push(d.getFullYear());
			lastYear = d.getFullYear();
		}
		if(lastMonth != d.getMonth())
		{
			name.push(['jan','feb','mar','apr','may','jun','jul','aug','sep','oct','nov','dec'][d.getMonth()]);
			lastMonth = d.getMonth();
		}
		name.push(d.getDate());
		topicsXml+=<a href={href}>[{name.join(' ')}]</a>;
	}
}

if(target.topics_navigate_page)
{
	topicsXml+=<><br/>по постоянным страницам последние 10 штук</>;
	
	let dbr = orm.query('SELECT DISTINCT page FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL AND page IS NOT NULL ORDER BY page DESC LIMIT 10', target.id);
	if(!dbr.length) dbr = [{page:1}];
	for each(let r in dbr)
	{
		let href = target.path+'/page'+r.page;
		topicsXml+=<a href={href}>[{r.page}]</a>;
	}
}

if(target.topics_navigate_rpage)
{
	topicsXml+=<><br/>по плывущим обратным страницам последние 10 штук</>;
	
	let dbr = orm.query('SELECT DISTINCT COUNT(*) FROM {ForumPost} WHERE forum_id=$1 AND tree_pid IS NULL', target.id);
	let count = dbr[0].count || 1;
	let rpageSize = target.topics_navigate_rpage_size_ || 20;
	let rpages = Math.ceil(count / rpageSize)
	for(let rpage=1; rpage<=rpages; rpage++)
	{
		let href = target.path+'/rpage'+rpage;
		topicsXml+=<a href={href}>[{rpage}]</a>;
	}
}


return <>

топики для форума {target.map_title} <br/>
топики разрешены: {target.topic_allow}<br/>
	{target.topic_allow?'тут статистика по топикам и навигатор к ним':''}


	<table border="1">{topicsXml}</table>
	<a href={forumPath+'/addTopic?backUrl='+forumPath+"/"+(new Date()).tsd}>addTopic</a>
	
</>
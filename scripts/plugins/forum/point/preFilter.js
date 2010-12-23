ui.blocks.breadcrumbs.front = this.parent.properties.breadcrumbs();


{
	let t = arguments.callee.header;
	if(!t)
	{
		t = router.createTemplate();
		t.compile(<>
			<div class="header-left">
				{t.logo}
			</div>
			<div class="header-right">
				{t.auth}
			</div>
			<div class="header-center">
				тут
			</div>
		</>);
		t.auth = router.getPoint('/user/authorize');
		arguments.callee.header = t;
	}
	t = t.clone();

	t.logo = ui.skin.image('/dog.png');
	t.auth = ui.skin.sidebar(t.auth.render(), "Авторизация");
	ui.blocks.header.push(t);


	let voting = <form>
		Сколько?<br/>
		
		<input type='radio' value='1'/> один<br/>
		<input type='radio' value='1' checked="1"/> четыре<br/>
		<input type='radio' value='1'/> сорок семь<br/>
		<input type='radio' value='1'/> больше ста<br/>
		<input type='submit' value='голос'/><br/>
	</form>;

	ui.blocks.right.push(ui.skin.sidebar(voting, 'Опрос'));
	ui.blocks.right.push(ui.skin.sidebar(voting, 'Блок справа'));
	ui.blocks.right.push(ui.skin.sidebar(voting, 'Блок справа'));


	let counter1 = <div style="width:70px;height:30px;background:gray;margin-left:0.3em;margin-right:0.3em;float:left"/>;
	ui.blocks.footer.push(counter1);
	ui.blocks.footer.push(counter1);
	ui.blocks.footer.push(counter1);
	ui.blocks.footer.push(<hr/>);
	ui.blocks.footer.push(<div style="float:right">Разработка и поддержка: ООО Рога и копыта</div>);


	ui.blocks.styles.forum = this.parent.path+'/main.css';




	ui.blocks.scripts.xregexp = "/js/xregexp.js";
	ui.blocks.scripts.highlight_core = "/syntaxhighlighter/src/shCore.js";
	ui.blocks.scripts.highlight_al = "/syntaxhighlighter/src/shAutoloader.js";
	ui.blocks.styles.highlight = '/syntaxhighlighter/styles/shCoreDefault.css';

	ui.blocks.scriptsTail.highlight_init = {body:<>
		SyntaxHighlighter.autoloader(
		'applescript /syntaxhighlighter/scripts/shBrushAppleScript.js',
		'actionscript3 as3 /syntaxhighlighter/scripts/shBrushAS3.js',
		'bash shell /syntaxhighlighter/scripts/shBrushBash.js',
		'coldfusion cf /syntaxhighlighter/scripts/shBrushColdFusion.js',
		'c cpp /syntaxhighlighter/scripts/shBrushCpp.js',
		'c# c-sharp csharp /syntaxhighlighter/scripts/shBrushCSharp.js',
		'css /syntaxhighlighter/scripts/shBrushCss.js',
		'delphi pascal pas /syntaxhighlighter/scripts/shBrushDelphi.js',
		'diff patch /syntaxhighlighter/scripts/shBrushDiff.js',
		'erl erlang /syntaxhighlighter/scripts/shBrushErlang.js',
		'groovy /syntaxhighlighter/scripts/shBrushGroovy.js',
		'java /syntaxhighlighter/scripts/shBrushJava.js',
		'jfx javafx /syntaxhighlighter/scripts/shBrushJavaFX.js',
		'js jscript javascript /syntaxhighlighter/scripts/shBrushJScript.js',
		'perl Perl pl /syntaxhighlighter/scripts/shBrushPerl.js',
		'php /syntaxhighlighter/scripts/shBrushPhp.js',
		'text plain /syntaxhighlighter/scripts/shBrushPlain.js',
		'powershell ps /syntaxhighlighter/scripts/shBrushPowerShell.js',
		'py python /syntaxhighlighter/scripts/shBrushPython.js',
		'ruby rails ror rb /syntaxhighlighter/scripts/shBrushRuby.js',
		'sass scss /syntaxhighlighter/scripts/shBrushSass.js',
		'scala /syntaxhighlighter/scripts/shBrushScala.js',
		'sql /syntaxhighlighter/scripts/shBrushSql.js',
		'vb vbnet /syntaxhighlighter/scripts/shBrushVb.js',
		'xml xhtml xslt html /syntaxhighlighter/scripts/shBrushXml.js',
		''
		);
		SyntaxHighlighter.config.bloggerMode = true;
		SyntaxHighlighter.all();
		</>};


/*  
*/

}



//пользователю время предыдущей сессии статистически
{
	if(user.login != 'anonymous')
	{
		let now = new Date();
			
		let key = 'forum.aime';
		let was = user.getData(key);
		if(was)
		{
			let delta = (now.getTime() - Date.fromTs(was).getTime())/1000;
			
			if(delta > 30*60*60)
			{
				user.setData('forum.lastSessionTime', now.ts);
			}
			user.setData(key, now.ts);
		}
		else
		{
			user.setData('forum.lastSessionTime', now.ts);
			user.setData(key, now.ts);
		}
	}

}
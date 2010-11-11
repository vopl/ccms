////////////////////////////////////////////////////////////
let system = 
{
	//системная бд, в ней все состояние
	db:undefined,
	
	//настройки различные
	options:
	{
		autoLoadContent:
		{
			valueData:false,
			get value(){return this.valueData},
			set value(v){this.valueData = (v?true:false)},
		},
		
		autoSaveContent:
		{
			valueData:false,
			get value(){return this.valueData},
			set value(v){this.valueData = v?true:false},
		},
		
		structPath:
		{
			valueData:'struct',
			get value(){return this.valueData},
			set value(v){this.valueData = String(v)},
		},
		
		i18nDirectories:
		{
			valueData:['i18n'],
			get value(){return this.valueData},
			set value(v)
			{
				if(!(v instanceof Array))
				{
					v = String(v).split(',');
				}
				this.valueData = v;
			},
		},

		pluginsPath:
		{
			valueData:'plugins',
			get value(){return this.valueData},
			set value(v){this.valueData = String(v)},
		},
		
		XML_ignoreComments:
		{
			get value(){return XML.ignoreComments},
			set value(v){XML.ignoreComments = v?true:false},
		},
		
		XML_ignoreProcessingInstructions:
		{
			get value(){return XML.ignoreProcessingInstructions},
			set value(v){XML.ignoreProcessingInstructions = v?true:false},
		},
		
		XML_prettyPrinting:
		{
			get value(){return XML.prettyPrinting},
			set value(v){XML.prettyPrinting = v?true:false},
		},
		
		XML_ignoreWhitespace:
		{
			get value(){return XML.ignoreWhitespace},
			set value(v){XML.ignoreWhitespace = v?true:false},
		},
		
		XML_prettyIndent:
		{
			get value(){return XML.prettyIndent},
			set value(v){XML.prettyIndent = Number(v)},
		},
	},
};

////////////////////////////////////////////////////////////
system.startup = function()
{
	this.db = router.createService('sqlite3');
	this.db.open("../var/system.sqlite");
	this.db.exec('PRAGMA encoding = "UTF-8"');
	this.db.exec('PRAGMA journal_mode = MEMORY');
	this.db.exec('PRAGMA temp_store = MEMORY');
	this.db.exec('PRAGMA foreign_keys = ON');

	let dbState = this.checkDbSchema();

	switch(dbState)
	{
	case 'normal':
		warn('system database in normal state, load content');
		this.loadFromDb_options();
		if(this.options.autoLoadContent.value)
		{
			this.loadFromDb();
		}
		else
		{
			this.loadFromDisk();
			if(this.options.autoSaveContent.value)
			{
				this.saveToDb();
			}
		}
		break;
	case 'inited':
		warn('system database inited, load content from disk');
		this.loadFromDisk_options();
		this.loadFromDisk();

		if(this.options.autoSaveContent.value)
		{
			this.saveToDb();
		}
		this.saveToDb_options();
		break;
	case 'wrong':
		throw Error("wrong system database state");
		break;
	default:
		throw Error("unknown system database state");
		break;
	};
};

////////////////////////////////////////////////////////////
system.shutdown = function()
{
	this.saveToDb_options();
	if(this.options.autoSaveContent.value)
	{
		this.saveToDb();
	}
	this.db.close();
	delete this.db;
	delete global.ui;
};

////////////////////////////////////////////////////////////
system.checkDbSchema = function()
{
	//проверка целостности базы
	let res = this.db.query('PRAGMA integrity_check;');
	if(res.length != 1 || res[0]['integrity_check'] != 'ok')
	{
		warn('integrity_check results for system database:');
		dumpe(res);
		return 'wrong';
	}


	//res = this.db.query("create table test(a integer)");
	res = this.db.query("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name");

	let tables = {};
	for each(let tname in res)
	{
		tname = tname['name'];
		tables[tname] = this.db.query("PRAGMA table_info("+tname+")");
	}

	if(!tables['options'] && !tables['objects'])
	{
		this.db.exec("CREATE TABLE options (name TEXT PRIMARY KEY, value TEXT)");
		this.db.exec("CREATE TABLE objects (id INTEGER PRIMARY KEY AUTOINCREMENT, kind TEXT, state TEXT, info TEXT)");
		return 'inited';
	}

	if(!tables['options'] || !tables['objects'])
	{
		return 'wrong';
	}

	return 'normal';
}

////////////////////////////////////////////////////////////
system.loadFromDb = function()
{
	this.loadFromDb_structure();
	this.loadFromDb_i18n();
	this.loadFromDb_ui();
	this.loadFromDb_plugins();
	warn('system.loadFromDb');
}

////////////////////////////////////////////////////////////
system.loadFromDb_structure = function()
{
	let res = this.db.query("SELECT state FROM objects WHERE kind='structure'");
	if(!res.length)
	{
		warn('structure absent in system database');
		return;
	}
	if(res.length>1)
	{
		warn('structure present more than one in system database, use first');
	}
	
	let conf = JSON.parse(res[0].state);
	let root = router.createPointInstance(conf);
	router.setPointInstance('/', root);

	
	warn('system.loadFromDb_structure');
}

////////////////////////////////////////////////////////////
system.loadFromDb_plugins = function()
{
	let res = this.db.query("SELECT state FROM objects WHERE kind='plugins'");
	if(!res.length)
	{
		warn('plugins absent in system database');
		return;
	}
	if(res.length>1)
	{
		warn('plugins present more than one in system database, use first');
	}

	let ser = JSON.parse(res[0].state);
	router.plugins.deserialize(ser);
	
	warn('system.loadFromDb_plugins');
}

////////////////////////////////////////////////////////////
system.loadFromDb_i18n = function()
{
	let res = this.db.query("SELECT state FROM objects WHERE kind='i18n'");
	if(!res.length)
	{
		warn('structure absent in system database');
		return;
	}
	if(res.length>1)
	{
		warn('structure present more than one in system database, use first');
	}
	
	let ser = JSON.parse(res[0].state);
	router.i18n.deserialize(ser);

	//поставщик текущей локали
	setL10nProvider(l10nProvider);
	
	warn('system.loadFromDb_i18n');
}

////////////////////////////////////////////////////////////
system.loadFromDb_ui = function()
{
	global.ui = router.createService('ui');
	global.uiPath = '/ui/';
	
	global.ui.skinProvider = uiSkinProvider;
	global.ui.layoutProvider = uiLayoutProvider;
	global.ui.blockProvider = uiBlockProvider;

	warn('system.loadFromDb_ui');
}

////////////////////////////////////////////////////////////
system.loadFromDb_options = function()
{
	let res = this.db.query('SELECT name, value FROM options');
	for each(let o in res)
	{
		if(o.name in this.options)
		{
			this.options[o.name].value = JSON.parse(o.value);
		}
		else
		{
			warn('system.loadFromDb_options: unknown option '+o.name);
		}
	}
	
	warn('system.loadFromDb_options')
}

////////////////////////////////////////////////////////////
system.loadFromDisk = function()
{
	this.loadFromDisk_structure();
	this.loadFromDisk_i18n();
	this.loadFromDisk_ui();
	this.loadFromDisk_plugins();
	
	warn('system.loadFromDisk');
}

////////////////////////////////////////////////////////////
system.loadFromDisk_structure = function()
{
	let root = router.createPointInstance(this.options.structPath.value+'/conf.js');
	router.setPointInstance('/', root);
	
	warn('system.loadFromDisk_structure');
}

////////////////////////////////////////////////////////////
system.loadFromDisk_plugins = function()
{
	while(router.plugins.directories.length)
	{
		router.plugins.directories.pop();
	}
	router.plugins.directories.push(this.options.pluginsPath.value);
	router.plugins.reload();

	warn('system.loadFromDisk_plugins');
}

////////////////////////////////////////////////////////////
system.loadFromDisk_i18n = function()
{
	while(router.i18n.directories.length) router.i18n.directories.pop();
	router.i18n.directories.join(this.options.i18nDirectories.value);
	router.i18n.reload();

	//поставщик текущей локали
	setL10nProvider(l10nProvider);

	warn('system.loadFromDisk_i18n');
}

////////////////////////////////////////////////////////////
system.loadFromDisk_ui = function()
{
	global.ui = router.createService('ui');
	global.uiPath = '/ui/';

	global.ui.skinProvider = uiSkinProvider;
	global.ui.layoutProvider = uiLayoutProvider;
	global.ui.blockProvider = uiBlockProvider;
	
	warn('system.loadFromDisk_ui');
}

////////////////////////////////////////////////////////////
system.loadFromDisk_options = function()
{
	this.options.autoLoadContent.value = false;
	this.options.autoSaveContent.value = false;
	this.options.structPath.value = 'struct';
	this.options.i18nDirectories.value = ['i18n'];
	this.options.pluginsPath.value = 'plugins';
	
	this.options.XML_ignoreComments.value = true;
	this.options.XML_ignoreProcessingInstructions.value = true;
	this.options.XML_prettyPrinting.value = true;
	this.options.XML_ignoreWhitespace.value = true;
	this.options.XML_prettyIndent.value = 2;
	
	warn('system.loadFromDisk_options');
}
////////////////////////////////////////////////////////////
system.saveToDb = function()
{
	this.saveToDb_plugins();
	this.saveToDb_ui();
	this.saveToDb_i18n();
	this.saveToDb_structure();

	warn('system.saveToDb');
}

////////////////////////////////////////////////////////////
system.saveToDb_structure = function()
{
	let conf = router.getPointConf(router.getPoint('/'));
	conf = JSON.stringify(conf);
	this.db.exec("DELETE FROM objects WHERE kind='structure'");
	this.db.exec("INSERT INTO objects (kind, state) VALUES ('structure', ?)", conf);
	warn('system.saveToDb_structure');
}

////////////////////////////////////////////////////////////
system.saveToDb_plugins = function()
{
	let ser = router.plugins.serialize();

	ser = JSON.stringify(ser);
	this.db.exec("DELETE FROM objects WHERE kind='plugins'");
	this.db.exec("INSERT INTO objects (kind, state) VALUES ('plugins', ?)", ser);

	warn('system.saveToDb_plugins');
}

////////////////////////////////////////////////////////////
system.saveToDb_ui = function()
{
	warn('system.saveToDb_ui');
}

////////////////////////////////////////////////////////////
system.saveToDb_i18n = function()
{
	let ser = router.i18n.serialize();
	ser = JSON.stringify(ser);
	this.db.exec("DELETE FROM objects WHERE kind='i18n'");
	this.db.exec("INSERT INTO objects (kind, state) VALUES ('i18n', ?)", ser);

	warn('system.saveToDb_i18n');
}

////////////////////////////////////////////////////////////
system.saveToDb_options = function()
{
	this.db.exec("DELETE FROM options");
	let s = this.db.prepare('INSERT INTO options (name, value) VALUES (?,?)');
	this.db.exec('BEGIN');
	for(let oname in this.options)
	{
		s.exec(oname, JSON.stringify(this.options[oname].value));
	}
	this.db.exec('COMMIT');
	
	warn('system.saveToDb_options');
}


return system;






















/////////////////////////////////////////////////////
function l10nProvider()
{
	var session = router.cd.session;
	if(session)
	{
		if(!session.l10n)
		{
			session.l10n = router.i18n.createL10n({
				name: 'default',
				text:
				{
					language:'ru',
					direction:'ltr',
					//inFiles: ['P:/projects/ccms/trunk/scripts/i18n/ru/f1.po'],
				},
				number:
				{
					delim0:'.',
					delim3:'',
					zero0:true,
					nan:'NAN',
					inf:'INF',
				},
				time:
				{
					tz:'Europe/Amsterdam',
				},
			});
		}
		
		if(!session.l10n.text)
		{
			session.l10n = router.i18n.createL10n(session.l10n.conf);
		}
	}
	else
	{
		throw Error("sessions must be enabled for use internationalization");
	}
	return session.l10n;
}







////////////////////////////////////////////////////
function uiSkinProvider()
{
	if(global.user)
	{
		//return global.user.settings('skin');
	}
	return 'default';
}



////////////////////////////////////////////////////
function uiLayoutProvider()
{
	if(global.user)
	{
		//return global.user.settings('layout');
	}
	return 'default';
}

////////////////////////////////////////////////////
function uiBlockProvider(name)
{
	if(global.user)
	{
		// let cfg = global.user.settings('block');
		// if(name in cfg)
		// {
			// let point = router.getPoint(cfg[name]);
			// if(point)
			// {
				// return point.render();
			// }
		// }
	}
	return undefined;
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////

return null;//warning
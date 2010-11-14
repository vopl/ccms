let iface = arguments[0];
if( !iface || 
	'object' != typeof iface ||
	iface.db != null ||
	iface.schema != 'public' ||
	'object' != typeof iface.categories)
{
	throw Error(absFile()+' for internal use only!!!');
}

///////////////////////////////////////////////////////////////////////
let impl = 
{
	iface:iface,
	fieldImpls:
	{
		'string':exec('fieldImpls/string.js'),
		'text':exec('fieldImpls/text.js'),

		'integer':exec('fieldImpls/integer.js'),
		'real':exec('fieldImpls/real.js'),

		'date':exec('fieldImpls/date.js'),

		'json':exec('fieldImpls/json.js'),
		'xml':exec('fieldImpls/xml.js'),

		'fkey':exec('fieldImpls/fkey.js'),
		'pkey':exec('fieldImpls/pkey.js'),

		'boolean':exec('fieldImpls/boolean.js'),
	},
	indexMethods:
	{
		'btree':true,
		'hash':true,
	},

	fkeyActions:
	{
		'cascade':true,
		'restrict':true,
		'setNull':true,
		'noAction':true,
	},
	
	tableoid2categoryImpl:{},
}



///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf = function prepareCategoryConf(conf)
{
	if(!conf.name) throw Error("name must be setted");
	if(conf.name in iface.categories) throw Error("category "+conf.name+" already exists");
	
	if(!conf.fields) conf.fields = {};
	if(!conf.indices) conf.indices = {};
	if(!conf.methods) conf.methods = {};
	if(!conf.categoryMethods) conf.categoryMethods = {};
	if(!conf.validator) conf.validator = null;
	if(!conf.inherits) conf.inherits = [];

	this.prepareCategoryConf_fields(conf, conf.fields);
	this.prepareCategoryConf_indices(conf, conf.indices);
	this.prepareCategoryConf_methods(conf, conf.methods);
	this.prepareCategoryConf_categoryMethods(conf, conf.categoryMethods);
	this.prepareCategoryConf_validator(conf, conf.validator);
	this.prepareCategoryConf_inherits(conf, conf);
	this.prepareCategoryConf_bases(conf, conf);
	
	// if(!conf.inherits.length)
	// {
		// if(conf.name != 'BaseCategory')
		// {
			// conf.inherits = ['BaseCategory'];
		// }
	// }
	
}

///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf_fields = function prepareCategoryConf_fields(conf, fields)
{
	for(let name in fields)
	{
		let value = fields[name];
		
		//если значение - просто строка то преобразовать в объект
		if('string' == typeof value)
		{
			value = {type:value};
			fields[name] = value;
		}

		//тип
		if(!(value.type in this.fieldImpls)) throw Error("bad type for field "+name);
		
		//дефолт
		if(!('default' in value)) value.default = null;
		
		//неПусто
		if(!('notNull' in value)) value.notNull = false;
		
		if(value.notNull && null==value.default) throw Error("default value for field "+name+" must not be a null");
		
		//валидатор
		if('validator' in value && 'function' != typeof value.validator) throw Error("validator for field "+name+" must be a function");

		//индекс
		if('index' in value)
		{
			if('string' == typeof value.index)
			{
				value.index = {unique:false, method:value.index};
			}
			value.index.targets = [name];
			
			if(!conf.indices) conf.indices = {};
			conf.indices['field_idx_'+name] = value.index;
			delete value.index;
		}
		
		if(value.type == 'fkey')
		{
			if(!value.target) throw Error("target must be setted for field "+name);
			if(!value.onDelete) value.onDelete = 'cascade';
			if(!(value.onDelete in this.fkeyActions)) throw Error("bad onDelete for field "+name);
			if(!value.onUpdate) value.onUpdate = 'cascade';
			if(!(value.onUpdate in this.fkeyActions)) throw Error("bad onUpdate for field "+name);
		}
	}
}

///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf_indices = function prepareCategoryConf_indices(conf, indices)
{
	for(let name in indices)
	{
		let value = indices[name];
		if('object' != typeof value) throw Error("index must be an object for "+name);
		if(!value.method) value.method = 'btree';
		if(!(value.method in this.indexMethods)) throw Error("bad index method "+value.method+" for index "+name);
		if(!value.unique) value.unique = false;
		
		if(!('targets' in value)) value.targets = [];
		if('target' in value)
		{
			value.targets.push(value.target);
			delete value.target;
		}
		
		if(!value.targets.length) throw Error("targets must be setted for index " + name);
		for each(let fname in value.targets)
		{
			if(!conf.fields[fname]) throw Error("target field "+fname+" for index "+name+" absent in category");
		}
	}
}

///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf_methods = function prepareCategoryConf_methods(conf, methods)
{
	for(let name in methods)
	{
		let value = methods[name];
		//ok
	}
}

///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf_categoryMethods = function prepareCategoryConf_categoryMethods(conf, methods)
{
	for(let name in methods)
	{
		let value = methods[name];
		//ok
	}
}

///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf_validator = function prepareCategoryConf_validator(conf, validator)
{
	//ok
}

///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf_inherits = function prepareCategoryConf_inherits(conf)
{
	if('string' == typeof conf.inherits)
	{
		conf.inherits = [conf.inherits];
	}
	if(!conf.inherits)
	{
		conf.inherits = [];
	}
	
	if(conf.inherit)
	{
		conf.inherits.push(conf.inherit);
		delete conf.inherit;
	}
	
	for(let idx in conf.inherits)
	{
		let value = conf.inherits[idx];
		if(value == 'BaseCategory') continue;
		
		if(!this.iface[value])
		{
			throw Error("base category "+value+" absent for "+conf.name);
		}
		
		if(this.iface[value].getImpl().conf.abstract)
		{
			throw Error("base category "+value+" for "+conf.name+' is abstract');
		}
	}
}


///////////////////////////////////////////////////////////////////////
impl.prepareCategoryConf_bases = function prepareCategoryConf_bases(conf)
{
	if('string' == typeof conf.bases)
	{
		conf.bases = [conf.bases];
	}
	if(!conf.bases)
	{
		conf.bases = [];
	}
	
	if(conf.base)
	{
		conf.bases.push(conf.base);
		delete conf.base;
	}
	
	for(let idx in conf.bases)
	{
		let value = conf.bases[idx];
		
		if(!this.iface[value])
		{
			if(value != 'BaseCategory')
			{
				throw Error("base category "+value+" absent for "+conf.name);
			}
		}
	}
}



///////////////////////////////////////////////////////////////////////
impl.addCategory = function addCategory(conf)
{
	let category = exec('category.js', conf, this);
	let categoryImpl = exec('categoryImpl.js', conf, category, this);
	
	//this.iface.categories[category.name] = category;
	
	return category;
}


///////////////////////////////////////////////////////////////////////
impl.delCategory = function delCategory(name)
{
	if(!this.iface.categories[name]) throw Error("wrong category name "+name);
	let category = this.iface.categories[name];
	let impl = category.getImpl();
	impl.destroy();
	
	return true;
}

///////////////////////////////////////////////////////////////////////
impl.sync2db = function sync2db(options)
{
	if(!this.iface.db) throw Error("db must be setted");
	
	if(!this.iface.categories['BaseCategory'])
	{
		let baseCategoryConf = exec('baseCategoryConf.js');
		this.iface.addCategory(baseCategoryConf);
	}
	
	this.iface.db.exec('BEGIN');
	try
	{
		//создать схему если небыло
		this.sync2db_createSchema(options);

		//создать генератор значений первичного ключа
		this.sync2db_createIdGenSequence(options);
		
		//выстроить очередность по иерархии наследования
		let categoryImpls = this.sync2db_linearizeCategories();

		//обновить иерархию
		for each(let categoryImpl in categoryImpls) categoryImpl.updateHierarchy();
		
		//выявить что какой либо из категорий нужно обновление базы
		let needSync = false;
		// for each(let categoryImpl in categoryImpls)
		// {
			// if(categoryImpl.needSync(options))
			// {
				// needSync = true;
				// break;
			// }
		// }
		needSync = true;


		if(needSync)
		{
			//бросить наследование
			for each(let categoryImpl in categoryImpls) categoryImpl.dropInheritance(options);
			
			//бросить ограничения
			for each(let categoryImpl in categoryImpls) categoryImpl.dropConstraints1(options);
			for each(let categoryImpl in categoryImpls) categoryImpl.dropConstraints2(options);
			
			//бросить индексы
			for each(let categoryImpl in categoryImpls) categoryImpl.dropIndices(options);
			
			
			
			
			//создать таблицы с полями
			//или модифицировать существующие
			for each(let categoryImpl in categoryImpls) categoryImpl.createTables(options);

			//корректировать данные
			for each(let categoryImpl in categoryImpls) categoryImpl.correctData(options);
			
			//создать ограничения
			for each(let categoryImpl in categoryImpls) categoryImpl.createConstraints1(options);
			for each(let categoryImpl in categoryImpls) categoryImpl.createConstraints2(options);
			
			//создать индексы
			for each(let categoryImpl in categoryImpls) categoryImpl.createIndices(options);
			
			//создать наследование
			for each(let categoryImpl in categoryImpls) categoryImpl.createInheritance(options);
		}		
		
		
		//бросить левые таблицы
		this.sync2db_dropLeftTables(options);

		this.tableoid2categoryImpl = {};
		for each(let categoryImpl in categoryImpls)
		{
			if(!categoryImpl.conf.abstract)
			{
				let tableoid = categoryImpl.selectTableoid();
				this.tableoid2categoryImpl[tableoid] = categoryImpl;
			}
		}
		
		for each(let categoryImpl in categoryImpls) categoryImpl.setSyncedFlag(true);
		
		this.iface.db.exec('COMMIT');
	}
	catch(e)
	{
		this.iface.db.exec('ROLLBACK');
		throw e;
	}
	
	return true;
}

///////////////////////////////////////////////////////////////////////
impl.sync2db_createSchema = function sync2db_createSchema(options)
{
	let db = this.iface.db;
	let dbr = db.query('SELECT * FROM information_schema.schemata WHERE schema_name=$1', this.iface.schema);
	if(!dbr.length)
	{
		if('allow' != options.createSchema)
		{
			throw Error("unable to create schema, options.createSchema != 'allow'");
		}
		
		db.exec('CREATE SCHEMA "'+this.iface.schema+'"');
	}
}

///////////////////////////////////////////////////////////////////////
impl.sync2db_createIdGenSequence = function sync2db_createIdGenSequence(options)
{
	let db = this.iface.db;
	let dbr = db.query('SELECT * FROM information_schema.sequences WHERE sequence_schema=$1 AND sequence_name=$2', this.iface.schema, 'common_id_seq');
	if(!dbr.length)
	{
		if('allow' != options.createSequence)
		{
			throw Error("unable to create sequence, options.createSequence != 'allow'");
		}
		
		db.exec('CREATE SEQUENCE "'+this.iface.schema+'"."common_id_seq"');
	}
}

///////////////////////////////////////////////////////////////////////
impl.sync2db_linearizeCategories = function sync2db_linearizeCategories(options)
{
	let processed = {};
	let result = [];
	
	let processor = function processor(categoryImpl)
	{
		if(categoryImpl.conf.name in processed) return true;
		
		let allBase = categoryImpl.getAllBases();
		for each(let base in allBase)
		{
			if(!processor(impl.iface))
			{
				return false;
			}
		}
		
		result.push(categoryImpl);
		processed[categoryImpl.conf.name] = true;
		return true;
	}
	
	for each(let category in this.iface.categories)
	{
		if(!processor(category.getImpl()))
		{
			throw Error("failed to determine linear sequence for categories inheritance");
		}
	}
	return result;
}


///////////////////////////////////////////////////////////////////////
impl.sync2db_dropLeftTables = function sync2db_dropLeftTables(options)
{
	let db = this.iface.db;
	for(;;)
	{
		let dbr = db.query('SELECT * FROM information_schema.tables WHERE table_schema=$1', this.iface.schema);
		let wasDrop = false;
		for each(let table in dbr)
		{
			let needDrop = false;
			if(!(table.table_name in this.iface.categories))
				needDrop = true;
				
			if(!needDrop && this.iface.categories[table.table_name].getImpl().conf.abstract)
				needDrop = true;
			
			if(needDrop)
			{
				if('allow' != options.dropTable)
				{
					warn("unable to drop table "+table.table_name+", options.dropTable != 'allow'");
					return;
				}
				
				wasDrop = true;
				db.exec('DROP TABLE "'+table.table_schema+'"."'+table.table_name+'" CASCADE');
				break;
			}
		}
		if(!wasDrop)
		{
			break;
		}
	}
}



/////////////////////////////////////////////////////////////////////////
impl.praparedStatements = {};
impl.getPraparedStatement = function getPraparedStatement(sql, key)
{
	if(!key) key = sql;
	let praparedStatements = this.praparedStatements;
	if(!(key in praparedStatements))
	{
		let db = this.iface.db;
		praparedStatements[key] = db.prepare(sql);
		
	}
	return praparedStatements[key];
}

impl.getPraparedStatementOnKey = function getPraparedStatementOnKey(key)
{
	let praparedStatements = this.praparedStatements;
	if(!(key in praparedStatements)) return undefined;
	return praparedStatements[key];
}

return impl;
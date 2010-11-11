let orm = 
{
	db:null,//экземпляр к базе
	schema:'public',//имя схемы в базе
	categories:{},//именованые категории
};

orm.__proto__ = orm.categories;

let impl = exec('ormImpl.js', orm);

////////////////////////////////////////////////////////
orm.addCategory = function addCategory(conf)
{
	// try
	{
		impl.prepareCategoryConf(conf);
		impl.addCategory(conf);
	}
	// catch(e)
	// {
		// throw Error("addCategory error: "+e+"\n"+dumps(conf));
	// }
}

////////////////////////////////////////////////////////
orm.delCategory = function delCategory(name)
{
	try
	{
		impl.delCategory(name);
	}
	catch(e)
	{
		throw Error("delCategory error: "+e+"\n"+name);
	}
}

////////////////////////////////////////////////////////
orm.setDb = function setDb(db)
{
	orm.db = db;
}

////////////////////////////////////////////////////////
orm.setSchema = function setSchema(schema)
{
	orm.schema = schema;
}

////////////////////////////////////////////////////////
//options.createSchema
//options.createSequence

//options.dropTable
//options.dropInheritance
//options.dropField
//options.dropIndex
//options.dropConstraint

//options.createTable
//options.createInheritance
//options.createField
//options.alterField
//options.createIndex
//options.createConstraint

//options.fillDefault
orm.sync2db = function sync2db(options)
{
	if(!options)
	{
		options = 
		{
			createSchema:'allow',
			createSequence:'allow',
			dropTable:'deny',
			dropInheritance:'allow',
			dropField:'deny',
			dropIndex:'allow',
			dropConstraint:'allow',
			
			createTable:'allow',
			createInheritance:'allow',
			createField:'allow',
			alterField:'allow',
			createIndex:'allow',
			createConstraint:'allow',
			
			fillDefault:'allow',
		};
	}
	// try
	{
		impl.sync2db(options);
	}
	// catch(e)
	// {
		// throw Error("sync2db error: "+e+"\n"+dumps(options));
	// }
}












/////////////////////////////////////////////
let regexp_categoryNameInSql = /\{(.*?)\}/gm;
orm.prepare = function query(sql)
{
	let stmt = impl.getPraparedStatementOnKey(sql);
	if(!stmt)
	{
		let sql2 = sql.replace(regexp_categoryNameInSql, function(str, name)
			{
				let category = orm[name];
				if(!category) throw Error("category name "+category+" in sql query is invalid ["+sql+"]")
				return category.getImpl().fullTableName;
			});
		stmt = impl.getPraparedStatement(sql2, sql);
	}
		
	return stmt;
}


/////////////////////////////////////////////
orm.query = function query(sql)
{
	let stmt = this.prepare(sql);
	let values = Array.prototype.slice.call(arguments, 1);
	let arr = stmt.query(values);
	
	return arr;
}

/////////////////////////////////////////////
orm.exec = function exec(sql)
{
	let stmt = this.prepare(sql);
	let values = Array.prototype.slice.call(arguments, 1);
	let arr = stmt.exec(values);
	
	return arr;
}
return orm;
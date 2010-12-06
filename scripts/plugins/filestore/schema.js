var orm = arguments[0];
var install = arguments[1];
var fc = arguments[2];

if(!install)
{
	orm.delCategory('File');
	return;
}


let createTriggerFile = function(orm)
{
	let dbr = orm.query('SELECT COUNT(*) AS count FROM information_schema.triggers WHERE trigger_schema=$1 AND trigger_name=$2 AND event_object_table=$3', orm.schema, 'FileCleanup_trigger', 'File');
	if(!dbr[0].count)
	{
		orm.exec('CREATE OR REPLACE FUNCTION "'+orm.schema+'"."FileCleanup_triggerFunctionFile"() RETURNS trigger AS $BODY$\n\
			BEGIN\n\
				 INSERT INTO "'+orm.schema+'"."FileCleanup" ("location") VALUES (old."location");\n\
				 RETURN old;\n\
			END $BODY$\n\
			LANGUAGE plpgsql;');

		orm.exec('CREATE TRIGGER "FileCleanup_trigger"\
			  AFTER DELETE\
			  ON "'+orm.schema+'"."File"\
			  FOR EACH ROW\
			  EXECUTE PROCEDURE "'+orm.schema+'"."FileCleanup_triggerFunctionFile"();');
	}
}

///////////////////////////////////////////////////////////
orm.addCategory({
	name:'File',
	inherits:'BaseCategory',
	fields:
	{
		ctime:'date',

		name:'string',
		size:'integer',
		type:'string',
		location:'string',
		
		comment:'text',
	},
	
	afterSync: function(orm){return createTriggerFile(orm)},

	methods:
	{
		setFromRequest:function setFromRequest(param, addData)
		{
			let newVals = {};
			
			newVals.type = /.*?(\.(.+))/.exec(param.nameClient)[2];
			newVals.type = newVals.type.toLowerCase();
			switch(newVals.type)
			{
			case 'zip':
			case 'rar':
			case 'gz':
			case 'bz':
			case 'bz2':

			case 'doc':
			case 'pdf':
			case 'txt':
			case 'djvu':
			
			case 'jpg':
			case 'jpeg':
			case 'gif':
			case 'png':
				break;
			default:
				this._error = "badType";
				return false;
			}
			
			newVals.location = hash.md5(Math.random(),Math.random(),Math.random(),uniq);
			newVals.location = newVals.location.replace(/(..)(..)(.*)/, "files/$1/$2/$3");
			newVals.location += '.'+newVals.type;
			
			try
			{
				fc.mkDir4File(newVals.location);
				fc.import(param.nameServer, newVals.location);
			}
			catch(e)
			{
				warn(e);
				this._error = "unableToSave";
				return false;
			}
			
			newVals.name = param.nameClient;
			newVals.size = param.size;
			
			if(undefined !== addData.comment && null !== addData.comment)
			{
				newVals.comment = addData.comment;
			}

			return this.set(newVals);
		},

		getError:function getError()
		{
			return this._error;
		},
	},
	categoryMethods:
	{
		makeFromRequest:function makeFromRequest(param, addData)
		{
			let rec = this.make({});
			rec.setFromRequest(param, addData);
			return rec;
		},
	},
});







let createTriggerImage = function(orm)
{
	let dbr = orm.query('SELECT COUNT(*) AS count FROM information_schema.triggers WHERE trigger_schema=$1 AND trigger_name=$2 AND event_object_table=$3', orm.schema, 'FileCleanup_trigger', 'Image');
	if(!dbr[0].count)
	{
		orm.exec('CREATE OR REPLACE FUNCTION "'+orm.schema+'"."FileCleanup_triggerFunctionImage"() RETURNS trigger AS $BODY$\n\
			BEGIN\n\
				 INSERT INTO "'+orm.schema+'"."FileCleanup" ("location") VALUES (old."location"), (old."location_thumb");\n\
				 RETURN old;\n\
			END $BODY$\n\
			LANGUAGE plpgsql;');

		orm.exec('CREATE TRIGGER "FileCleanup_trigger"\
			  AFTER DELETE\
			  ON "'+orm.schema+'"."Image"\
			  FOR EACH ROW\
			  EXECUTE PROCEDURE "'+orm.schema+'"."FileCleanup_triggerFunctionImage"();');
	}
}


///////////////////////////////////////////////////////////
orm.addCategory({
	name:'Image',
	inherits:'File',
	fields:
	{
		width:'integer',
		height:'integer',
		
		location_thumb:'string',
		width_thumb:'integer',
		height_thumb:'integer',
	},

	afterSync: function(orm){return createTriggerImage(orm)},
	methods:
	{
		setFromRequest:function setFromRequest(param, addData)
		{
			let newVals = {};
			newVals.type = /.*?(\.(.+))/.exec(param.nameClient)[2];
			newVals.type = newVals.type.toLowerCase();
			switch(newVals.type)
			{
			case 'jpg':
			case 'jpeg':
			case 'gif':
			case 'png':
				break;
			default:
				this._error = "badType";
				return false;
			}
			
			
			newVals.name = param.nameClient;
			newVals.size = param.size;
			if(undefined !== addData.comment && null !== addData.comment)
			{
				newVals.comment = addData.comment;
			}
			
			
			//взять файл из param
			let mi;
			try
			{
				mi = new Magick.Image(param.nameServer);
			}
			catch(e)
			{
				warn(e);
				this._error = "unknownFormat";
				return false;
			}

			switch(mi.magick())
			{
			case 'PNG': break;
				if('png' != newVals.type)
				{
					this._error = "unmachedFormat";
					return false;
				}
				break;
			case 'JPEG': break;
				if('jpeg' != newVals.type && 'jpg' != newVals.type)
				{
					this._error = "unmachedFormat";
					return false;
				}
				break;
			case 'GIF': break;
				if('gif' != newVals.type)
				{
					this._error = "unmachedFormat";
					return false;
				}
				break;
			default: _error = "badFormat"; return false;
			}
			
			//для thumb экземпляр
			let mit = Magick.Image(mi);
			
			//масштабировать в addData.width x addData.height
			if(addData.width || addData.height)
			{
				mi.scale((addData.width?addData.width:'')+'x'+(addData.height?addData.height:''));
			}
			newVals.width = mi.attributes().columns();
			newVals.height = mi.attributes().rows();

			//масштабировать в addData.width x addData.height
			if(addData.width_thumb || addData.height_thumb)
			{
				mit.scale((addData.width_thumb?addData.width_thumb:'')+'x'+(addData.height_thumb?addData.height_thumb:''));
			}
			else
			{
				mit.scale('100x100');
			}
			newVals.width_thumb = mi.attributes().columns();
			newVals.height_thumb = mi.attributes().rows();

			//сохранить
			newVals.location = hash.md5(Math.random(),Math.random(),Math.random(),uniq);
			newVals.location = newVals.location.replace(/(..)(..)(.*)/, "images/$1/$2/$3");
			newVals.location_thumb = newVals.location+'.thumb';
			newVals.location += '.'+newVals.type;
			newVals.location_thumb += '.'+newVals.type;
			
			try
			{
				fc.mkDir4File(newVals.location);
				mi.write(fc.pathAbs(newVals.location));
				//newVals.location = absFile(newVals.location);
				mit.write(fc.pathAbs(newVals.location_thumb));
				//newVals.location_thumb = absFile(newVals.location_thumb);
			}
			catch(e)
			{
				this._error = "unableToSave";
				return false;
			}
			
			return this.set(newVals);
		},
	},
});




///////////////////////////////////////////////////////////
orm.addCategory({
	name:'FileCleanup',
	fields:
	{
		location:'string',
	},
	
	categoryMethods:
	{
		doCleanup:function doCleanup()
		{
			let dbr = orm.query('SELECT * FROM {FileCleanup}');
			for each(let rec in dbr)
			{
				if(fc.isFile(rec.location))
				{
					fc.rm(rec.location, true);
				}
			}
			orm.exec('DELETE FROM {FileCleanup}');
		},
	},
	
})
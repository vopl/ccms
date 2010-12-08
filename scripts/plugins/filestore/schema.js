var orm = arguments[0];
var install = arguments[1];
var fcs = global.fcs;

if(!install)
{
	orm.delCategory('File');
	orm.delCategory('Image');
	orm.delCategory('FileCleanup');
	return;
}


let createTriggerFile = function(orm)
{
	orm.exec('DROP TRIGGER IF EXISTS "FileCleanup_trigger" ON "'+orm.schema+'"."File" CASCADE');
	
	orm.exec('CREATE OR REPLACE FUNCTION "'+orm.schema+'"."FileCleanup_triggerFunctionFile"() RETURNS trigger AS $BODY$\n\
		BEGIN\n\
			 INSERT INTO "'+orm.schema+'"."FileCleanup" ("fc", "location") VALUES (old."fc", old."location");\n\
			 RETURN old;\n\
		END $BODY$\n\
		LANGUAGE plpgsql;');

	orm.exec('CREATE TRIGGER "FileCleanup_trigger"\
		  AFTER DELETE\
		  ON "'+orm.schema+'"."File"\
		  FOR EACH ROW\
		  EXECUTE PROCEDURE "'+orm.schema+'"."FileCleanup_triggerFunctionFile"();');
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

		fc:'string',
		location:'string',
		
		comment:'text',
	},
	
	afterSync: function(orm){return createTriggerFile(orm)},

	methods:
	{
		setFromRequest:function setFromRequest(param, addData, fc)
		{
			if(!fc) fc = 'public';
			
			if(!(fc in fcs))
			{
				throw Error("wrong fileContainer ("+fc+"), see keys in global.fcs");
			}
			
			
			let newVals = {fc:fc};
			fc = global.fcs[fc];
			
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
			
			newVals.location = crypto.rand.str(32);
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

			
			if(this.fc)
			{
				if(this.location)
				{
					orm.exec("INSERT INTO {FileCleanup} (fc, location) VALUES($1, $2)", this.fc, this.location);
				}
			}
			
			let res = this.set(newVals);
			return res;
		},

		getError:function getError()
		{
			return this._error;
		},
	},
	categoryMethods:
	{
		makeFromRequest:function makeFromRequest(param, addData, fc)
		{
			let rec = this.make({});
			rec.setFromRequest(param, addData, fc);
			return rec;
		},
	},
});







let createTriggerImage = function(orm)
{
	orm.exec('DROP TRIGGER IF EXISTS "FileCleanup_trigger" ON "'+orm.schema+'"."Image" CASCADE');
	
	orm.exec('CREATE OR REPLACE FUNCTION "'+orm.schema+'"."FileCleanup_triggerFunctionImage"() RETURNS trigger AS $BODY$\n\
		BEGIN\n\
			 INSERT INTO "'+orm.schema+'"."FileCleanup" ("fc", "location") VALUES (old."fc", old."location"), (old."fc", old."location_thumb");\n\
			 RETURN old;\n\
		END $BODY$\n\
		LANGUAGE plpgsql;');

	orm.exec('CREATE TRIGGER "FileCleanup_trigger"\
		  AFTER DELETE\
		  ON "'+orm.schema+'"."Image"\
		  FOR EACH ROW\
		  EXECUTE PROCEDURE "'+orm.schema+'"."FileCleanup_triggerFunctionImage"();');
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
		setFromRequest:function setFromRequest(param, addData, fc)
		{
			if(!fc) fc = 'public';
			
			if(!(fc in fcs))
			{
				throw Error("wrong fileContainer ("+fc+"), see keys in global.fcs");
			}
			
			
			let newVals = {fc:fc};
			fc = global.fcs[fc];
			
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
				let needScale = false;
				if(addData.width && addData.height)
				{
					if(mi.attributes().columns()!=addData.width || mi.attributes().rows()!=addData.height)
					{
						needScale = true;
					}
				}
				else if(addData.width)
				{
					if(mi.attributes().columns()!=addData.width)
					{
						needScale = true;
					}
				}
				else if(addData.height)
				{
					if(mi.attributes().rows()!=addData.height)
					{
						needScale = true;
					}
				}
				
				if(needScale)
				{
					mi.scale((addData.width?addData.width:'')+'x'+(addData.height?addData.height:''));
				}
			}
			newVals.width = mi.attributes().columns();
			newVals.height = mi.attributes().rows();

			//масштабировать в addData.width x addData.height
			if(addData.width_thumb || addData.height_thumb)
			{
				let needScale = false;
				if(addData.width_thumb && addData.height_thumb)
				{
					if(mi.attributes().columns()!=addData.width_thumb || mi.attributes().rows()!=addData.height_thumb)
					{
						needScale = true;
					}
				}
				else if(addData.width_thumb)
				{
					if(mi.attributes().columns()!=addData.width_thumb)
					{
						needScale = true;
					}
				}
				else if(addData.height_thumb)
				{
					if(mi.attributes().rows()!=addData.height_thumb)
					{
						needScale = true;
					}
				}
				
				if(needScale)
				{
					mit.scale((addData.width_thumb?addData.width_thumb:'')+'x'+(addData.height_thumb?addData.height_thumb:''));
				}
			}
			else
			{
				mit.scale('100x100');
			}
			newVals.width_thumb = mit.attributes().columns();
			newVals.height_thumb = mit.attributes().rows();

			//сохранить
			newVals.location = crypto.rand.str(32);
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
			
			if(this.fc)
			{
				if(this.location)
				{
					orm.exec("INSERT INTO {FileCleanup} (fc, location) VALUES($1, $2)", this.fc, this.location);
				}
				if(this.location_thumb)
				{
					orm.exec("INSERT INTO {FileCleanup} (fc, location) VALUES($1, $2)", this.fc, this.location_thumb);
				}
			}
			
			let res = this.set(newVals);
			return res;
		},
	},
});




///////////////////////////////////////////////////////////
orm.addCategory({
	name:'FileCleanup',
	fields:
	{
		fc:'string',
		location:'string',
	},
	
	categoryMethods:
	{
		doCleanup:function doCleanup()
		{
			let dbr = orm.query('SELECT * FROM {FileCleanup}');
			for each(let rec in dbr)
			{
				if(rec.fc in global.fcs)
				{
					let fc = global.fcs[rec.fc];
					if(fc.isFile(rec.location))
					{
						fc.rm(rec.location);
					}
				}
				else
				{
					warn("unknown fc: " + dumps(rec));
				}
			}
			orm.exec('DELETE FROM {FileCleanup}');
		},
	},
	
})
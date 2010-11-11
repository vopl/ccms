/*
тут управление плугом

инсталяция деинсталяция
запуск останов экземпляров











плуг есть некоторая программа со своими запчастями, а именно orm, таблицы бд, структура точек, другие штуки

все плуги лежат в одном месте для плугов
идентифицируются уникальным именем

в структуре есть выделенное место для структур плугов
?ps/user/tra/ta/ta
?pis/user/user/tra/ta/ta
?pis/user/moderator/tra/ta/ta
?pis/user/admin/tra/ta/ta


все плуги, которые доступные к инсталяции или проинсталированы, находятся тут 
router.plugins;

for(var plugName in router.plugins) {...}//перечисление
router.plugins.user.isInstalled == false;

//инсталяция
router.plugins.user.install();//доступен/нет
//при инсталяции плуг создает себе схему в orm или бд, делает другие манипуляции


//доступ к плугу
router.plugins.user.tratata ...
//плуг может установить себя в глобе в выделенном хэше ps (PluginS)
global.ps.user.tratata ...
//или просто
ps.user.tratata ...

//запуск экземпляра
router.plugins['user'].start('user');
router.plugins['user'].start('moderator');
router.plugins['user'].start('administartor');
//доступ к экземпляру
router.plugins['user'].instances['user1'].tratata ...

//плуг может установить экземпляр в глобе в выделенном хэше pis (Plugin InstanceS)
global.pis.user1.tratata ...
//или просто
pis.user1.tratata ...

//останов экземпляра и деинсталяция
router.plugins['user'].stop('user');
router.plugins['user'].stop('moderator');
router.plugins['user'].stop('administrator');
router.plugins.user.uninstall();


//все эти манипуляции могут бросать исключения
*/

var plug=
{
	name:'base',
	
	revision:0,
	dependences:{},
	users:{},
	
	isInstalled:false,
	isInitialized:false,

	////////////////////////////////////////////////////////////////////
	installImpl:function(){},
	install:function()
	{
		//если уже проинсталирован то ничего не делать
		if(this.isInstalled)
		{
			return true;
		}
		
		//проверить чтобы все зависимости присутствовали
		for(var dependencyName in this.dependences)
		{
			var dependent = router.plugins[dependencyName];
			var needRevision = this.dependences[dependencyName];
			if(!dependent)
			{
				throw Error("dependent plugin '"+dependencyName+"' for '"+this.name+"' is absent");
			}
			if(dependent.revision < needRevision)
			{
				throw Error("dependent plugin '"+dependencyName+"'("+dependent.revision+") for '"+this.name+"' has too small revision, need "+needRevision);
			}
		}
		
		//запустить все зависимости
		for(var dependencyName in this.dependences)
		{
			var dependent = router.plugins[dependencyName];
			if(!dependent.isInstalled)
			{
				throw Error("dependent plugin '"+dependencyName+"' for '"+this.name+"' is not installed");
			}
		}
		
		//положить свой объект в global.ps
		if(!global.ps) global.ps = {};
		global.ps[this.name] = this;

		//позвать реализацию
		try
		{
			this.isInstalled = true;
			this.isInstalled = this.installImpl.apply(this);
		}
		catch(e)
		{
			this.isInstalled = false;
			throw e;
		}
		
		return this.isInstalled;
	},

	////////////////////////////////////////////////////////////////////
	uninstallImpl:function(){},
	uninstall:function()
	{
		//если не инсталирован то ничего и не делать
		if(!this.isInstalled)
		{
			return true;
		}
		
		//если есть проинсталированные использующие то ошибка
		if(this.users)
		{
			for each(var user in this.users)
			{
				if(user.isInstalled)
				{
					throw Error("unable to uninstall '"+this.name+"', used from '"+user.name+"'");
				}
			}
		}
		
		//удалить себя из зависимостей
		for(var dependencyName in this.dependences)
		{
			var dependent = router.plugins[dependencyName];
			if(dependent.users)
			{
				delete dependent.users[this.name];
			}
		}

		//позвать реализацию
		try
		{
			this.uninstallImpl.apply(this);
		}
		catch(e)
		{
			throw e;
		}
		
		//удалить свой объект из global.ps
		if(global.ps && global.ps[this.name])
		{
			delete global.ps[this.name];
		}
		this.isInstalled = false;
		return true;
	},












	////////////////////////////////////////////////////////////////////
	serializeImpl:function(){},
	serialize:function()
	{
		if(this.isInstalled)
		{
			let res =
			{
				data:this.serializeImpl(),
			};

			return res;
		};

		return undefined;
	},
	
	////////////////////////////////////////////////////////////////////
	deserializeImpl:function(conf){return true;},
	deserialize:function(conf, iter)
	{
		for(var dependencyName in this.dependences)
		{
			var dependent = router.plugins[dependencyName];
			if(!dependent)
			{
				throw Error("dependent plugin '"+dependencyName+"' for '"+this.name+"' is absent");
			}
			if(!dependent.isInstalled)
			{
				return false;
				//throw Error("dependent plugin '"+dependencyName+"' for '"+this.name+"' is not installed");
			}
			
			var needRevision = this.dependences[dependencyName];
			if(dependent.revision < needRevision)
			{
				throw Error("dependent plugin '"+dependencyName+"'("+dependent.revision+") for '"+this.name+"' has too small revision, need "+needRevision);
			}
		}
	
		this.isInstalled = true;
		
		if(!this.deserializeImpl(conf.data))
		{
			return false;
		}

		this.isInstalled = true;
		return true;
	},


		////////////////////////////////////////////////////////////////////
	initializeImpl:function(){return true;},
	initialize:function()
	{
		if(this.isInstalled && !this.isInitialized)
		{
			//запустить все зависимости
			for(var dependencyName in this.dependences)
			{
				var dependent = router.plugins[dependencyName];
				if(!dependent.isInstalled)
				{
					throw Error("dependent plugin '"+dependencyName+"' for '"+this.name+"' is not installed");
				}
				
				if(!dependent.isInitialized)
				{
					dependent.initialize();
				}
			}
		
			this.initializeImpl();
			this.isInitialized = true;
		}
		return true;
	}
};
return plug;
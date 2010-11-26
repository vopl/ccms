let baseCategory = 
{
	name:'BaseCategory',
	abstract:true,
	fields:
	{
		id:'pkey'
	},
	
	methods:
	{
		load:function()
		{
			let category = orm[this.category];
			if(!category) throw Error('unknown category '+this.category);

			return category.load(this);
		},
		set:function(v)
		{
			let category = orm[this.category];
			if(!category) throw Error('unknown category '+this.category);

			return category.set(this, v);
		},
		save:function()
		{
			let category = orm[this.category];
			if(!category) throw Error('unknown category '+this.category);

			return category.save(this);
		},
		delete:function()
		{
			let category = orm[this.category];
			if(!category) throw Error('unknown category '+this.category);

			return category.delete(this);
		},
	},

	categoryMethods:
	{
		//////////////////////////////////////////////////////////
		//сформировать персистентный объект из внешнего хэша или массива хэшей
		make: function make(v)
		{
			return this.getImpl().make(v);
		},

		//////////////////////////////////////////////////////////
		adopt: function adopt(v)
		{
			return this.getImpl().adopt(v);
		},

		//////////////////////////////////////////////////////////
		set: function set(dst, src)
		{
			return this.getImpl().set(dst, src);
		},

		//////////////////////////////////////////////////////////
		insert: function insert(v)
		{
			return this.getImpl().insert(v);
		},

		//////////////////////////////////////////////////////////
		getLastInsertId: function getLastInsertId()
		{
			return this.getImpl().getLastInsertId();
		},

		//////////////////////////////////////////////////////////
		getNextInsertId: function getNextInsertId()
		{
			return this.getImpl().getNextInsertId();
		},

		//////////////////////////////////////////////////////////
		update: function update(v)
		{
			return this.getImpl().update(v);
		},

		//////////////////////////////////////////////////////////
		save: function save(v)
		{
			if(v.id)
			{
				return this.update(v);
			}

			return this.insert(v);
		},

		//////////////////////////////////////////////////////////
		delete: function delete(v)
		{
			return this.getImpl().delete(v);
		},


		//////////////////////////////////////////////////////////
		load: function load(v)
		{
			return this.getImpl().load(v);
		},

		//////////////////////////////////////////////////////////
		select: function select()
		{
			let impl = this.getImpl();
			return impl.select.apply(impl, arguments);
		},
	},
};

//////////////////////////////////////////////////////////////////
return baseCategory;
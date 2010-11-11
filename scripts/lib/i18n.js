//////////////////////////////////////////////////////////////////
//поставщик текущей локали по замыканию
var l10nProvider = function()
{
	throw Error("provider for l10n must be setted");
}

//////////////////////////////////////////////////////////////////
global.__defineGetter__('_c', function()
{
	return l10nProvider().conf;
});


//////////////////////////////////////////////////////////////////
global._ = function()
{
	var lazy = {args:Array.prototype.slice.call(arguments)};
	lazy.toString = function()
	{
		var l10n = l10nProvider();
		return l10n.text.apply(l10n, lazy.args);
	};
	return lazy;
	
}
ro(global, '_');

//////////////////////////////////////////////////////////////////
global._n = function()
{
	var lazy = {args:Array.prototype.slice.call(arguments)};
	lazy.toString = function()
	{
		var l10n = l10nProvider();
		return l10n.number.apply(l10n, lazy.args);
	};
	return lazy;
}
ro(global, '_n');

//////////////////////////////////////////////////////////////////
global._t = function()
{
	var lazy = {args:Array.prototype.slice.call(arguments)};
	lazy.toString = function()
	{
		var l10n = l10nProvider();
		return l10n.time.apply(l10n, lazy.args);
	};
	return lazy;
}
ro(global, '_t');

//////////////////////////////////////////////////////////////////
global.setL10nProvider = function(provider)
{
	l10nProvider = provider;
}
ro(global, 'setL10nProvider');

//////////////////////////////////////////////////////////////////
global.getL10nProvider = function()
{
	return setL10nProvider;
}
ro(global, 'getL10nProvider');

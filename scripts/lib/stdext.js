
Array.prototype.__defineGetter__('back', function()
{
	if(!this.length) return undefined;
	return this[this.length-1];
});
he(Array.prototype, 'back');





Array.prototype.__defineGetter__('front', function()
{
	if(!this.length) return undefined;
	return this[0];
});
he(Array.prototype, 'front');

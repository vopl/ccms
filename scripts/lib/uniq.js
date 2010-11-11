(function()
{
	var cnt=1;
	global.__defineGetter__('uniq', function(){return cnt++});
	ro(global, 'uniq');
})();

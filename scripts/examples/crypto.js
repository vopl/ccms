function f()
{
	let report = {};
	report.dataJson = {a:'строка', b:[], c:0, d:{}};
	report.data = "Начальное состояние s0 называется инициализирующим вектором.";
	report.pswd = Crypto.Rand.str_(8);
	
	function test(name)
	{
		if(!(name in crypto)) return;

		let alg = crypto[name];
		
		let lr = {};
		lr.code = alg.encode(report.pswd, report.data);
		lr.codeJson = alg.encodeJson(report.pswd, report.dataJson);
		lr.res = alg.decode(report.pswd, lr.code);
		lr.resJson = alg.decodeJson(report.pswd, lr.codeJson);
		lr.bpres = alg.decode(report.pswd+'b', lr.code);
		lr.bpresJson = alg.decodeJson(report.pswd+'b', lr.codeJson);
		lr.bdres = alg.decode(report.pswd, lr.code+'b');
		lr.bdresJson = alg.decodeJson(report.pswd, lr.codeJson+'b');
		
		report[name] = lr;
	}
	

	test('Des');
	test('Aes');
	test('Blowfish');
	test('Cast');
	test('Idea');
	test('Rc2');
	test('Rc4');
	
	return dumps(report);
}

//let teman = router.plugins.texteditor.manager;

//let te = teman.getTe(this.path, {engine:'tmce', option2:'value2'});
return <pre>{f.toString()}<br/>f(): {f()}<br/>dumps(crypto):{dumps(crypto)}</pre>;//+te.render();
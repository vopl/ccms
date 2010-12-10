function f()
{
	let dataJson = {a:'строка', b:[], c:0, d:{}};
	let data = JSON.stringify(dataJson);
	let pswd = crypto.rand.str_(20);
	
	
	let report = {};
	report.dataJson = {a:'строка', b:[], c:0, d:{}};
	report.data = JSON.stringify(dataJson);
	report.pswd = crypto.rand.str_(8);
	
	function test(name)
	{
		let alg = crypto[name];
		
		let lr = {};
		lr.code = alg.encrypt(report.pswd, report.data);
		lr.codeJson = alg.encryptJson(report.pswd, report.dataJson);
		lr.res = alg.decrypt(report.pswd, lr.code);
		lr.resJson = alg.decryptJson(report.pswd, lr.codeJson);
		
		report[name] = lr;
	}
	

	test('des');
	test('aes');
	test('blowfish');
	test('cast');
	test('idea');
	test('rc2');
	test('rc4');
	
	return dumps(report);
}

//let teman = router.plugins.texteditor.manager;

//let te = teman.getTe(this.path, {engine:'tmce', option2:'value2'});
return <pre>{f.toString()}{f()}{dumps(crypto)}</pre>;//+te.render();
function f()
{
	let data = {a:'строка', b:[], c:0, d:{}};
	let pswd = crypto.rand.str_(8);
	let code1 = crypto.aes.encryptJson(pswd, data);
	let code2 = crypto.des.encryptJson(pswd, data);

	let res1 = crypto.aes.decryptJson(pswd, code1);
	let res2 = crypto.des.decryptJson(pswd, code2);

	return dumps([data, pswd, code1, code2, res1, res2]);
}

//let teman = router.plugins.texteditor.manager;

//let te = teman.getTe(this.path, {engine:'tmce', option2:'value2'});
return <pre>{f.toString()}{f()}</pre>;//+te.render();
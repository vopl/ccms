function f()
{
	let dataJson = {a:'строка', b:[], c:0, d:{}};
	let data = JSON.stringify(dataJson);
	let pswd = crypto.rand.str_(20);
	let code1 = crypto.aes.encrypt(pswd, data);
	let code2 = crypto.des.encrypt(pswd, data);
	let codeJson1 = crypto.aes.encryptJson(pswd, dataJson);
	let codeJson2 = crypto.des.encryptJson(pswd, dataJson);

	let res1 = crypto.aes.decrypt(pswd, code1);
	let res2 = crypto.des.decrypt(pswd, code2);
	let resJson1 = crypto.aes.decryptJson(pswd, codeJson1);
	let resJson2 = crypto.des.decryptJson(pswd, codeJson2);

	return dumps([data, dataJson, pswd, code1, code2, codeJson1, codeJson2, res1, res2, resJson1, resJson2]);
}

//let teman = router.plugins.texteditor.manager;

//let te = teman.getTe(this.path, {engine:'tmce', option2:'value2'});
return <pre>{f.toString()}{f()}</pre>;//+te.render();
warn(crypto.sync.aes.generateKey());
		let pswd = crypto.rand.str_(43);
		let msg = {msg:crypto.rand.str_(4),a:'b',c:220};
		let caes = crypto.sync.aes.encodeJson(pswd, msg);
		let cdes = crypto.sync.des.encodeJson(pswd, msg);
		let msg2 = crypto.sync.aes.decodeJson(pswd, caes);
		dumpe([msg, cdes, caes, JSON.stringify(msg), msg2]);

return;


let teman = router.plugins.texteditor.manager;

let te = teman.getTe(this.path, {engine:'tmce', option2:'value2'});
return te.render();
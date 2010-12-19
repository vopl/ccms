let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'superscript';
e.tag = 'sup';
e.parser.tags[e.tag]=0;

return e;
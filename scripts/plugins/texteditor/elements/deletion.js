let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'deletion';
e.tag = 'del';
e.parser.tags[e.tag]=0;

return e;
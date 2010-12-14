let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'insertion';
e.tag = 'ins';
e.parser.tags[e.tag]=0;

return e;
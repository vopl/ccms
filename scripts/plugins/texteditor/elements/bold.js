let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'bold';
e.tag = 'b';
e.parser.tags[e.tag]=0;

return e;
let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'subscript';
e.tag = 'sub';
e.parser.tags[e.tag]=0;

return e;
let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'italic';
e.tag = 'i';
e.parser.tags[e.tag]=0;

return e;
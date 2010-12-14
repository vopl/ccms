let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'italic';
e.tag = 'em';
e.parser.tags[e.tag]=0;

return e;
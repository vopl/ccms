let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'paragraph';
e.tag = 'p';
e.parser.tags[e.tag]=0;

return e;
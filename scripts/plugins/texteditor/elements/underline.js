let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'underline';
e.tag = 'u';
e.parser.tags[e.tag]=0;

return e;
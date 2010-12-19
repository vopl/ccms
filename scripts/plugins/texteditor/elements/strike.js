let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'strike';
e.tag = 's';
e.parser.tags[e.tag]=0;

return e;
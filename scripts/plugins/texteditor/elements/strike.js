let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'strike';
e.tag = 'strike';
e.parser.tags[e.tag]=0;

return e;
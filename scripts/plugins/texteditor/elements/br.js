let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'br';
e.tag = 'br';
e.parser.tags[e.tag]=0;
e.parser.noChilds=true;

return e;
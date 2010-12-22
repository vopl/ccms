let e = exec('simpleTag.js');

///////////////////////////////////////////////
e.kind = 'hr';
e.tag = 'hr';
e.parser.tags[e.tag]=0;
e.parser.noChilds=true;

return e;
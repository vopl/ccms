//return <div>data is not setted at {this.path}</div>;
let name = this.properties.placeName();
if(name in ui.blocks) return ui.blocks[name];

let target = request.plan[request.plan.length-1];

if('title' in target.point.properties) return target.point.properties.title();

return "title for "+target.path;


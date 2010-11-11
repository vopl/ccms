//return <div>data is not setted at {this.path}</div>;
let name = this.properties.placeName();
if(name in ui.blocks) return ui.blocks[name];
return undefined;


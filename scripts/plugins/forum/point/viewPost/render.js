ui.blocks.scripts.jquery144='/js/jquery-1.4.4.js';
let res = this.properties.renderRow(request.planData.post);

ui.blocks.scriptsTail.postTreeManager = this.parent.path+'/postTreeManager.js';
ui.blocks.scriptsTail.postTreeManagerInit = {body:<><![CDATA[window.ccms.postTreeManager.init();]]></>};
return res;

return <div class='forum-tree-man'>
	горизонтально <br/>

<label for="forum-tree-man-hmin">min</label><input type="radio" name='forum-tree-man-h' id="forum-tree-man-hmin" onclick='window.ccms.postTreeManager.set("hmin")'/>
<label for="forum-tree-man-hauto">auto</label><input type="radio" name='forum-tree-man-h' id="forum-tree-man-hauto" onclick='window.ccms.postTreeManager.set("hauto")'/>
<label for="forum-tree-man-hmax">max</label><input type="radio" name='forum-tree-man-h' id="forum-tree-man-hmax" onclick='window.ccms.postTreeManager.set("hmax")'/>

	<br/>
	вертикально <br/>
<input type="button" id="forum-tree-man-vmin" value="свернуть все" onclick='window.ccms.postTreeManager.set("vmin")'/>
<input type="button" id="forum-tree-man-vmax"  value="развернуть все" onclick='window.ccms.postTreeManager.set("vmax")'/>
	<br/>

	обновления<br/>
<input type="button" id="forum-tree-man-update" value="обновить" onclick='window.ccms.postTreeManager.update()'/>
	<br/>

</div>;
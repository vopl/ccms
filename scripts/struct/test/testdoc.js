let doc =<><h1>tinymce.AddOnManager--------</h1>
<p class="classDetailsContent"><h1>tinymce.AddOnManager</h1>
<table class="classDetails">
<tr>
<td class="first">Namespace</td>
<td class="last">tinymce</td>
</tr>
<tr>
<td class="first">Class</td>
<td class="last">AddOnManager</td>
</tr>
</table>
<p class="classDescription">This class handles the loading of themes/plugins or other add-ons and their language packs.
</p>
<p class="summaryLists">
<h2>Public Methods</h2>
<table class="Methods summary">
<thead>
<tr>
<th>Method</th>
<th>Defined By</th>
</tr>
</thead>
<tbody>
<tr>
<td class="first">
<p>
<a class="memberName" href="class_tinymce.AddOnManager.html#add">add</a>(id:String, o:<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a>):<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a>
</p>
<p class="summary">Adds a instance of the add-on by it's short name.</p>
</td>
<td class="last">AddOnManager</td>
</tr>
<tr class="even">
<td class="first">
<p>
<a class="memberName" href="class_tinymce.AddOnManager.html#get">get</a>(n:String):<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a>
</p>
<p class="summary">Returns the specified add on by the short name.</p>
</td>
<td class="last">AddOnManager</td>
</tr>
<tr>
<td class="first">
<p>
<a class="memberName" href="class_tinymce.AddOnManager.html#load">load</a>(n:String, u:String, cb:function, s:Object):void</p>
<p class="summary">Loads an add-on from a specific url.</p>
</td>
<td class="last">AddOnManager</td>
</tr>
<tr class="even">
<td class="first">
<p>
<a class="memberName" href="class_tinymce.AddOnManager.html#requireLangPack">requireLangPack</a>(n:String):void</p>
<p class="summary">Loads a language pack for the specified add-on.</p>
</td>
<td class="last">AddOnManager</td>
</tr>
</tbody>
</table>
<h2>Public Events</h2>
<table class="Events summary">
<thead>
<tr>
<th>Event</th>
<th>Defined By</th>
</tr>
</thead>
<tbody>
<tr>
<td class="first">
<p>
<a class="memberName" href="class_tinymce.AddOnManager.html#onAdd">onAdd</a>()</p>
<p class="summary">Fires when a item is added.</p>
</td>
<td class="last">AddOnManager</td>
</tr>
</tbody>
</table>
</p>
<p class="detailsList">
<p class="details">
<h2>Method details</h2>
<p class="memberDetails" id="add">
<h3>add<span class="memberType">method</span>
</h3>
<code class="syntax">public 
						function add(id:String, o:<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a>):<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a>
</code>
<p class="memberDescription">Adds a instance of the add-on by it's short name.
</p>
<h4>Parameters</h4>
<table class="params">
<tr>
<td class="first">id:String</td>
<td class="last">Short name/id for the add-on.</td>
</tr>
<tr>
<td class="first">o:<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a>
</td>
<td class="last">Theme or plugin to add.</td>
</tr>
</table>
<h4>Returns</h4>
<p class="returns">
<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a> - The same theme or plugin instance that got passed in.</p>
</p>
<p class="memberDetails" id="get">
<h3>get<span class="memberType">method</span>
</h3>
<code class="syntax">public 
						function get(n:String):<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a>
</code>
<p class="memberDescription">Returns the specified add on by the short name.
</p>
<h4>Parameters</h4>
<table class="params">
<tr>
<td class="first">n:String</td>
<td class="last">Add-on to look for.</td>
</tr>
</table>
<h4>Returns</h4>
<p class="returns">
<a href="class_tinymce.Theme.html">Theme</a>/<a href="class_tinymce.Plugin.html">Plugin</a> - Theme or plugin add-on instance or undefined.</p>
</p>
<p class="memberDetails" id="load">
<h3>load<span class="memberType">method</span>
</h3>
<code class="syntax">public 
						function load(n:String, u:String, cb:function, s:Object):void</code>
<p class="memberDescription">Loads an add-on from a specific url.
</p>
<h4>Parameters</h4>
<table class="params">
<tr>
<td class="first">n:String</td>
<td class="last">Short name of the add-on that gets loaded.</td>
</tr>
<tr>
<td class="first">u:String</td>
<td class="last">URL to the add-on that will get loaded.</td>
</tr>
<tr>
<td class="first">cb:function</td>
<td class="last">Optional callback to execute ones the add-on is loaded.</td>
</tr>
<tr>
<td class="first">s:Object</td>
<td class="last">Optional scope to execute the callback in.</td>
</tr>
</table>
</p>
<p class="memberDetails last" id="requireLangPack">
<h3>requireLangPack<span class="memberType">method</span>
</h3>
<code class="syntax">public 
						function requireLangPack(n:String):void</code>
<p class="memberDescription">Loads a language pack for the specified add-on.
</p>
<h4>Parameters</h4>
<table class="params">
<tr>
<td class="first">n:String</td>
<td class="last">Short name of the add-on.</td>
</tr>
</table>
</p>
</p>
<p class="details">
<h2>Event details</h2>
<p class="memberDetails last" id="onAdd">
<h3>onAdd<span class="memberType">event</span>
</h3>
<code class="syntax">public 
						event onAdd()</code>
<p class="memberDescription">Fires when a item is added.
</p>
</p>
</p>
</p>
</p> </>;

return doc;
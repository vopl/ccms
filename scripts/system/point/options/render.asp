<%
	function option(name, option)
	{
		let value = option.value;
	%>
	<tr>
		<td>
			<%=esc.str(name)%>
		</td>
		<td>
			<%=typeof value%>
		</td>
		<td>
			<form method='POST'>
				<input type='hidden' name='name' value='<%=esc.str(name)%>'>
				<%
				switch(typeof value)
				{
				case 'boolean':
					%><input type='checkbox' name='value' value='1' <%=value?'checked':''%>><%
					break;
				case 'string':
				default:
					%><input type='text' name='value' value='<%=esc.str(value)%>'><%
					break;
				case 'object':
					if('length' in value)
					{//array
						%><input type='text' name='value' value='<%=esc.str(value)%>'><%
					}
					else
					{
						%><input type='text' name='value' value='<%=esc.str(value)%>'><%
					}
					break;
				}
				%>
				<input type='submit' value='set'>
			</form>
		</td>
	</tr>
	<%
	}
%>

<table>
<%
	for(let name in system.options)
	{
		option(name, system.options[name]);
	}
%>

</table>


			<form method='POST'>
				<input type='hidden' name='saveToBase' value='1'>
				<input type='submit' value='save to base'>
			</form>


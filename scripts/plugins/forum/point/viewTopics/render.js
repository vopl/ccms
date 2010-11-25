if(request.planData.date)
{
	return "as date "+request.planData.date;
}

if(request.planData.page)
{
	return "as page "+request.planData.page;
}


if(request.planData.rpage)
{
	return "as rpage "+request.planData.rpage;
}

return <>
	таблица<br/>
	навигатор на соседние<br/>
	навигатор на другой метод, страница которого содержит первый пост данного метода<br/>
	навигатор на форум<br/>
</>;
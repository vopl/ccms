let mostPost = request.planData.post;
let mostTopic = request.planData.topic;


let res = (mostTopic==mostPost?'Тема: ':'Ответ: ')+mostPost.title;

return res;

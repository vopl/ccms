var conf=
{
	name: 'ru',

	text:
	{
		//язык по ISO 639, http://ru.wikipedia.org/wiki/Коды_языков
		language:'ru',
		//http://ru.wikipedia.org/wiki/ISO_3166-1
		country:'RU',

		//направление письма
		direction:'ltr',

		//папка с po mo файлами переводов строк
		inDirectories: absDir('trans'),

		//отдельные файлы po mo
		inFiles: [absFile('f1.po'), absFile('f2.mo')],

	},

	number:
	{

		formats:
		{
			int:'i',
			real:'g',

			bin:'i2',
			oct:'i8',
			dec:'i',
			hex:'i16',
			HEX:'i16U',

			custom1:function(num)
			{
				//this равен атрибуту number конфа локали
				return ""+this.nan+num;
			}
		},

		//разделитель целой дробной части
		delim0:'.',

		//разделитель 3 разрядов
		delim3:'',

		//ведущий ноль в дрбном<1
		zero0:true,

		//не число
		nan:'NAN',

		//бесконечность
		inf:'INF',

		//http://ru.wikipedia.org/wiki/Приставки_СИ
		factors10:
		{
			'-24':'y',
			'-21':'z',
			'-18':'a',
			'-15':'f',
			'-12':'p',
			'-9':'n',
			'-6':'u',
			'-3':'m',
			'-2':'c',
			'-1':'d',

			1:'da',
			2:'h',
			3:'k',
			6:'M',
			9:'G',
			12:'T',
			15:'P',
			18:'E',
			21:'Z',
			24:'Y',
		},
		//http://ru.wikipedia.org/wiki/Двоичные_приставки
		factors2:
		{
			10:'Ki',
			20:'Mi',
			30:'Gi',
			40:'Ti',
			50:'Pi',
			60:'Ei',
			70:'Zi',
			80:'Yi',
		},
	},

	time:
	{
		formats:
		{
			date0:"Y-m-d H:i:s",
			date1:function(t){return "f: "+this.tz+t},
			date2:"dd.MM",
			date3:"dd.MM",
			date4:"dd.MM",
			time0:"hh.mm",
			time1:"hh.mm.ss",
			time2:"hh.mm",
			time3:"hh.mm",
			time4:"hh.mm",
			datetime0:"dd.MM hh.mm",
			datetime1:"dd.MM hh.mm",
			datetime2:"dd.MM hh.mm",
			datetime3:"dd.MM hh.mm",
			datetime4:"dd.MM hh.mm",
		},

		tz:'Europe/Moscow',
		//tz:'Pacific/Saipan',

		dowShort:['вс', 'пн', 'вт', 'ср', 'чт', 'пт', 'сб'],
		dowLong:['воскресенье', 'понедельник', 'вторник', 'среда', 'четверг', 'пятница', 'суббота'],
		monthLong:['январь','февраль','мерт','апрель','май','июнь','июль','август','сентябрь','октябрь','ноябрь','декабрь',],
		monthShort:['янв','фев','мер','апр','май','июн','июл','авг','сен','окт','ноя','дек',],
		am:'am',
		pm:'pm',
		AM:'AM',
		PM:'PM',
	},
};
return conf;
//dumpe(Magick);

// let image = new Magick.Image('c:/temp/2.bmp');
// let image2 = new Magick.Image('P:/sdk/ImageMagick-6.6.4/contrib/win32/MFC/NtMagick/res/ImageMagick.bmp');
//let image2 = new Magick.Image('c:/temp/2.bmp');

//image2.adaptiveBlur(0.1, 1.34);
//image2.adaptiveThreshold(10, 20, -4);
//image2.addNoise('random');
//image2.addNoiseChannel(Magick.RedChannel, Magick.RandomNoise);
//image2.affineTransform(0.1, 0.5, 0.2, 0.5, 0.6, 0.1);
//image2.affineTransform(new Magick.Affine(0.1, 0.5, 0.2, 0.5, 0.6, 0.1));
//image2.annotate('рус', Magick.SouthWestGravity);
//image2.blurChannel(Magick.RGBChannels, 0.2, 1.2);
//image2.border("5x10+4+20");
//image2.channel(Magick.RedChannel);

//image2.channelDepth(Magick.RedChannel, 16);
//let t = image2.channelDepth(Magick.RedChannel);
//dumpe(t);

//image2.charcoal();

//image2.chop("20x30");
//image2.cdl('tartata');

//image2.colorize(1000, 1000, 0, '#010203');
//image2.colorMatrix([2.435, 1.3, 1.5, 2.4]);

//image2.comment('tratata');

//image2.compose(Magick.XorCompositeOp);
//let t = image2.compare(image);
//dumpe(t);

//image2.composite(image, Magick.WestGravity);

//image2.contrast(0);
/*
image2.convolve([
	2.435, 1.3, 1.5, 
	2.435, 1.3, 1.5, 
	2.435, 1.3, 1.5, 
	]);
*/

//image2.crop('10x50');
//image2.cycleColormap(56);
//image2.display();
//image2.distort(Magick.ArcDistortion, [0.1, 0.2, 0.3, 0.4, 0.5, 0.6], true);

/*
image2.draw(Magick.DrawableArc(0, 0, 50, 50, -90, 180));
image2.draw(Magick.DrawableText(10,100, "тратата"));

image2.draw(Magick.DrawableArc(0, 0, 50, 50, -90, 180));
let dl = [
	Magick.DrawableRectangle(100, 100, 110, 120), 
	Magick.DrawableCircle(75,100, 80,110)];
image2.draw(dl);
*/


//image2.edge(0);
//image2.emboss();
//image2.enhance();
//image2.equalize();
//image2.erase();
//image2.extent("80x180", Magick.NorthWestGravity, '#ffff0e');
//image2.flip();

//image2.floodFillColor('20x45', '#ff0505');
//image2.floodFillColor(20, 45, '#ff0505', '#000000');
//image2.floodFillOpacity(10, 10, 5, Magick.FillToBorderMethod)
//image2.floodFillTexture(20, 45, image, '#000000');
//image2.flop();
//image2.frame(25, 45, 5, 5);
//image2.fx('(1.0/(1.0+exp(10.0*(0.5-u)))-0.006693)*1.0092503', Magick.AllChannels);
//warn(image2.gamma(0.4));
//image2.gaussianBlur(5, 0.5);
//image2.inverseFourierTransform(image);
//warn(image2.label("tratata рус"));
//image2.level(0.5, 1.7, 1.1);
//image2.levelChannel(Magick.RedChannel, 0.5, 1.7, 1.1);
//image2.magnify();
//image2.map(image);
//image2.matteFloodfill('#0105ff', 10, 50, 50, Magick.FloodfillMethod);
//image2.medianFilter(5.2);
//image2.minify();
//image2.modulate(0.5, 1.5, 1.0);
//image2.motionBlur(5, 1.0, 45);
//image2.negate(true);
//image2.normalize();
//image2.oilPaint();
//image2.ping('C:/temp/2.bmp');
//image2.quantize();
//image2.process('IM_MOD_DB_bgr_', 'arg1', 'arg2');
//image2.read('20x20', 'C:/temp/2.bmp');
//dumpe(image2.statistics());
//dumpe(image2.attributes().image() == image2);


//image.write("c:/temp/original.bmp");
//image2.write("c:/temp/mod.bmp");

//warn('done');
//return;


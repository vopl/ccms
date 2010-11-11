#include "magick_pch.h"
#include "magick/magick.hpp"
#include "magick/image.hpp"
#include "magick/geometry.hpp"
#include "magick/color.hpp"

#include "magick/drawable.hpp"
#include "magick/path.hpp"

namespace ccms{ namespace magick{

	//////////////////////////////////////////////////////////////////////////
	Magick::Magick()
		: JsObject(true, "[object Magick]")
	{
		jsval jsv;

		jsv = (new ImageConstructor)->thisJsval();
		jsRegisterProp("Image", jsv, true);

		jsv = (new GeometryConstructor)->thisJsval();
		jsRegisterProp("Geometry", jsv, true);

		jsv = (new ColorConstructor)->thisJsval();
		jsRegisterProp("Color", jsv, true);






		jsv = (new DrawableAffineConstructor)->thisJsval();				jsRegisterProp("DrawableAffine", jsv, true);
		jsv = (new DrawableArcConstructor)->thisJsval();				jsRegisterProp("DrawableArc", jsv, true);
		jsv = (new DrawableBezierConstructor)->thisJsval();				jsRegisterProp("DrawableBezier", jsv, true);
		jsv = (new DrawableClipPathConstructor)->thisJsval();			jsRegisterProp("DrawableClipPath", jsv, true);
		jsv = (new DrawableCircleConstructor)->thisJsval();				jsRegisterProp("DrawableCircle", jsv, true);
		jsv = (new DrawableColorConstructor)->thisJsval();				jsRegisterProp("DrawableColor", jsv, true);
		jsv = (new DrawableCompositeImageConstructor)->thisJsval();		jsRegisterProp("DrawableCompositeImage", jsv, true);
		jsv = (new DrawableDashArrayConstructor)->thisJsval();			jsRegisterProp("DrawableDashArray", jsv, true);
		jsv = (new DrawableDashOffsetConstructor)->thisJsval();			jsRegisterProp("DrawableDashOffset", jsv, true);
		jsv = (new DrawableEllipseConstructor)->thisJsval();			jsRegisterProp("DrawableEllipse", jsv, true);
		jsv = (new DrawableFillColorConstructor)->thisJsval();			jsRegisterProp("DrawableFillColor", jsv, true);
		jsv = (new DrawableFillRuleConstructor)->thisJsval();			jsRegisterProp("DrawableFillRule", jsv, true);
		jsv = (new DrawableFillOpacityConstructor)->thisJsval();		jsRegisterProp("DrawableFillOpacity", jsv, true);
		jsv = (new DrawableFontConstructor)->thisJsval();				jsRegisterProp("DrawableFont", jsv, true);
		jsv = (new DrawableGravityConstructor)->thisJsval();			jsRegisterProp("DrawableGravity", jsv, true);
		jsv = (new DrawableLineConstructor)->thisJsval();				jsRegisterProp("DrawableLine", jsv, true);
		jsv = (new DrawableMatteConstructor)->thisJsval();				jsRegisterProp("DrawableMatte", jsv, true);
		jsv = (new DrawableMiterLimitConstructor)->thisJsval();			jsRegisterProp("DrawableMiterLimit", jsv, true);
		jsv = (new DrawablePathConstructor)->thisJsval();				jsRegisterProp("DrawablePath", jsv, true);
		jsv = (new DrawablePointConstructor)->thisJsval();				jsRegisterProp("DrawablePoint", jsv, true);
		jsv = (new DrawablePointSizeConstructor)->thisJsval();			jsRegisterProp("DrawablePointSize", jsv, true);
		jsv = (new DrawablePolygonConstructor)->thisJsval();			jsRegisterProp("DrawablePolygon", jsv, true);
		jsv = (new DrawablePolylineConstructor)->thisJsval();			jsRegisterProp("DrawablePolyline", jsv, true);
		jsv = (new DrawablePopClipPathConstructor)->thisJsval();		jsRegisterProp("DrawablePopClipPath", jsv, true);
		jsv = (new DrawablePopGraphicContextConstructor)->thisJsval();	jsRegisterProp("DrawablePopGraphicContext", jsv, true);
		jsv = (new DrawablePushClipPathConstructor)->thisJsval();		jsRegisterProp("DrawablePushClipPath", jsv, true);
		jsv = (new DrawablePushGraphicContextConstructor)->thisJsval();	jsRegisterProp("DrawablePushGraphicContext", jsv, true);
		jsv = (new DrawablePushPatternConstructor)->thisJsval();		jsRegisterProp("DrawablePushPattern", jsv, true);
		jsv = (new DrawablePopPatternConstructor)->thisJsval();			jsRegisterProp("DrawablePopPattern", jsv, true);
		jsv = (new DrawableRectangleConstructor)->thisJsval();			jsRegisterProp("DrawableRectangle", jsv, true);
		jsv = (new DrawableRotationConstructor)->thisJsval();			jsRegisterProp("DrawableRotation", jsv, true);
		jsv = (new DrawableRoundRectangleConstructor)->thisJsval();		jsRegisterProp("DrawableRoundRectangle", jsv, true);
		jsv = (new DrawableScalingConstructor)->thisJsval();			jsRegisterProp("DrawableScaling", jsv, true);
		jsv = (new DrawableSkewXConstructor)->thisJsval();				jsRegisterProp("DrawableSkewX", jsv, true);
		jsv = (new DrawableSkewYConstructor)->thisJsval();				jsRegisterProp("DrawableSkewY", jsv, true);
		jsv = (new DrawableStrokeAntialiasConstructor)->thisJsval();	jsRegisterProp("DrawableStrokeAntialias", jsv, true);
		jsv = (new DrawableStrokeColorConstructor)->thisJsval();		jsRegisterProp("DrawableStrokeColor", jsv, true);
		jsv = (new DrawableStrokeLineCapConstructor)->thisJsval();		jsRegisterProp("DrawableStrokeLineCap", jsv, true);
		jsv = (new DrawableStrokeLineJoinConstructor)->thisJsval();		jsRegisterProp("DrawableStrokeLineJoin", jsv, true);
		jsv = (new DrawableStrokeOpacityConstructor)->thisJsval();		jsRegisterProp("DrawableStrokeOpacity", jsv, true);
		jsv = (new DrawableStrokeWidthConstructor)->thisJsval();		jsRegisterProp("DrawableStrokeWidth", jsv, true);
		jsv = (new DrawableTextConstructor)->thisJsval();				jsRegisterProp("DrawableText", jsv, true);
		jsv = (new DrawableTextAntialiasConstructor)->thisJsval();		jsRegisterProp("DrawableTextAntialias", jsv, true);
		jsv = (new DrawableTextDecorationConstructor)->thisJsval();		jsRegisterProp("DrawableTextDecoration", jsv, true);
		jsv = (new DrawableTextUnderColorConstructor)->thisJsval();		jsRegisterProp("DrawableTextUnderColor", jsv, true);
		jsv = (new DrawableTranslationConstructor)->thisJsval();		jsRegisterProp("DrawableTranslation", jsv, true);
		jsv = (new DrawableViewboxConstructor)->thisJsval();			jsRegisterProp("DrawableViewbox", jsv, true);






		jsv = (new PathArcAbsConstructor)->thisJsval();			jsRegisterProp("PathArcAbs", jsv, true);
		jsv = (new PathArcRelConstructor)->thisJsval();			jsRegisterProp("PathArcRel", jsv, true);

		jsv = (new PathClosePathConstructor)->thisJsval();			jsRegisterProp("PathClosePath", jsv, true);

		jsv = (new PathCurveToAbsConstructor)->thisJsval();			jsRegisterProp("PathCurveToAbs", jsv, true);
		jsv = (new PathCurveToRelConstructor)->thisJsval();			jsRegisterProp("PathCurveToRel", jsv, true);

		jsv = (new PathLineToAbsConstructor)->thisJsval();			jsRegisterProp("PathLineToAbs", jsv, true);
		jsv = (new PathLineToRelConstructor)->thisJsval();			jsRegisterProp("PathLineToRel", jsv, true);

		jsv = (new PathLineToHorizontalAbsConstructor)->thisJsval();			jsRegisterProp("PathLineToHorizontalAbs", jsv, true);
		jsv = (new PathLineToHorizontalRelConstructor)->thisJsval();			jsRegisterProp("PathLineToHorizontalRel", jsv, true);

		jsv = (new PathLineToVerticalAbsConstructor)->thisJsval();			jsRegisterProp("PathLineToVerticalAbs", jsv, true);
		jsv = (new PathLineToVerticalRelConstructor)->thisJsval();			jsRegisterProp("PathLineToVerticalRel", jsv, true);

		jsv = (new PathMoveToAbsConstructor)->thisJsval();			jsRegisterProp("PathMoveToAbs", jsv, true);
		jsv = (new PathMoveToRelConstructor)->thisJsval();			jsRegisterProp("PathMoveToRel", jsv, true);

		jsv = (new PathQuadraticCurveToAbsConstructor)->thisJsval();			jsRegisterProp("PathQuadraticCurveToAbs", jsv, true);
		jsv = (new PathQuadraticCurveToRelConstructor)->thisJsval();			jsRegisterProp("PathQuadraticCurveToRel", jsv, true);

		jsv = (new PathSmoothCurveToAbsConstructor)->thisJsval();			jsRegisterProp("PathSmoothCurveToAbs", jsv, true);
		jsv = (new PathSmoothCurveToRelConstructor)->thisJsval();			jsRegisterProp("PathSmoothCurveToRel", jsv, true);

		jsv = (new PathSmoothQuadraticCurveToAbsConstructor)->thisJsval();			jsRegisterProp("PathSmoothQuadraticCurveToAbs", jsv, true);
		jsv = (new PathSmoothQuadraticCurveToRelConstructor)->thisJsval();			jsRegisterProp("PathSmoothQuadraticCurveToRel", jsv, true);


#define MKENUMVAL(val) jsRegisterProp(#val,	INT_TO_JSVAL(MagickCore::val), true);


		MKENUMVAL(UndefinedChannel);
		MKENUMVAL(RedChannel);
		MKENUMVAL(RGBChannels);
		MKENUMVAL(GrayChannel);
		MKENUMVAL(GreenChannel);
		MKENUMVAL(GrayChannels);
		MKENUMVAL(CyanChannel);
		MKENUMVAL(MagentaChannel);
		MKENUMVAL(MatteChannel);
		MKENUMVAL(BlueChannel);
		MKENUMVAL(BlackChannel);
		MKENUMVAL(YellowChannel);
		MKENUMVAL(AlphaChannel);
		MKENUMVAL(AllChannels);
		MKENUMVAL(OpacityChannel);
		MKENUMVAL(IndexChannel);
		MKENUMVAL(TrueAlphaChannel);
		MKENUMVAL(SyncChannels);
		MKENUMVAL(DefaultChannels);





		MKENUMVAL(UndefinedNoise);
		MKENUMVAL(UniformNoise);
		MKENUMVAL(GaussianNoise);
		MKENUMVAL(MultiplicativeGaussianNoise);
		MKENUMVAL(ImpulseNoise);
		MKENUMVAL(LaplacianNoise);
		MKENUMVAL(PoissonNoise);
		MKENUMVAL(RandomNoise);







		MKENUMVAL(UndefinedGravity);
		MKENUMVAL(ForgetGravity);
		MKENUMVAL(NorthWestGravity);
		MKENUMVAL(NorthGravity);
		MKENUMVAL(NorthEastGravity);
		MKENUMVAL(WestGravity);
		MKENUMVAL(CenterGravity);
		MKENUMVAL(EastGravity);
		MKENUMVAL(SouthWestGravity);
		MKENUMVAL(SouthGravity);
		MKENUMVAL(SouthEastGravity);
		MKENUMVAL(StaticGravity);











		MKENUMVAL(UndefinedCompositeOp);
		MKENUMVAL(NoCompositeOp);
		MKENUMVAL(ModulusAddCompositeOp);
		MKENUMVAL(AtopCompositeOp);
		MKENUMVAL(BlendCompositeOp);
		MKENUMVAL(BumpmapCompositeOp);
		MKENUMVAL(ChangeMaskCompositeOp);
		MKENUMVAL(ClearCompositeOp);
		MKENUMVAL(ColorBurnCompositeOp);
		MKENUMVAL(ColorDodgeCompositeOp);
		MKENUMVAL(ColorizeCompositeOp);
		MKENUMVAL(CopyBlackCompositeOp);
		MKENUMVAL(CopyBlueCompositeOp);
		MKENUMVAL(CopyCompositeOp);
		MKENUMVAL(CopyCyanCompositeOp);
		MKENUMVAL(CopyGreenCompositeOp);
		MKENUMVAL(CopyMagentaCompositeOp);
		MKENUMVAL(CopyOpacityCompositeOp);
		MKENUMVAL(CopyRedCompositeOp);
		MKENUMVAL(CopyYellowCompositeOp);
		MKENUMVAL(DarkenCompositeOp);
		MKENUMVAL(DstAtopCompositeOp);
		MKENUMVAL(DstCompositeOp);
		MKENUMVAL(DstInCompositeOp);
		MKENUMVAL(DstOutCompositeOp);
		MKENUMVAL(DstOverCompositeOp);
		MKENUMVAL(DifferenceCompositeOp);
		MKENUMVAL(DisplaceCompositeOp);
		MKENUMVAL(DissolveCompositeOp);
		MKENUMVAL(ExclusionCompositeOp);
		MKENUMVAL(HardLightCompositeOp);
		MKENUMVAL(HueCompositeOp);
		MKENUMVAL(InCompositeOp);
		MKENUMVAL(LightenCompositeOp);
		MKENUMVAL(LinearLightCompositeOp);
		MKENUMVAL(LuminizeCompositeOp);
		MKENUMVAL(MinusCompositeOp);
		MKENUMVAL(ModulateCompositeOp);
		MKENUMVAL(MultiplyCompositeOp);
		MKENUMVAL(OutCompositeOp);
		MKENUMVAL(OverCompositeOp);
		MKENUMVAL(OverlayCompositeOp);
		MKENUMVAL(PlusCompositeOp);
		MKENUMVAL(ReplaceCompositeOp);
		MKENUMVAL(SaturateCompositeOp);
		MKENUMVAL(ScreenCompositeOp);
		MKENUMVAL(SoftLightCompositeOp);
		MKENUMVAL(SrcAtopCompositeOp);
		MKENUMVAL(SrcCompositeOp);
		MKENUMVAL(SrcInCompositeOp);
		MKENUMVAL(SrcOutCompositeOp);
		MKENUMVAL(SrcOverCompositeOp);
		MKENUMVAL(ModulusSubtractCompositeOp);
		MKENUMVAL(ThresholdCompositeOp);
		MKENUMVAL(XorCompositeOp);
		MKENUMVAL(DivideCompositeOp);
		MKENUMVAL(DistortCompositeOp);
		MKENUMVAL(BlurCompositeOp);
		MKENUMVAL(PegtopLightCompositeOp);
		MKENUMVAL(VividLightCompositeOp);
		MKENUMVAL(PinLightCompositeOp);
		MKENUMVAL(LinearDodgeCompositeOp);
		MKENUMVAL(LinearBurnCompositeOp);
		MKENUMVAL(MathematicsCompositeOp);








		MKENUMVAL(UndefinedDistortion);
		MKENUMVAL(AffineDistortion);
		MKENUMVAL(AffineProjectionDistortion);
		MKENUMVAL(ScaleRotateTranslateDistortion);
		MKENUMVAL(PerspectiveDistortion);
		MKENUMVAL(PerspectiveProjectionDistortion);
		MKENUMVAL(BilinearForwardDistortion);
		MKENUMVAL(BilinearDistortion);
		MKENUMVAL(BilinearReverseDistortion);
		MKENUMVAL(PolynomialDistortion);
		MKENUMVAL(ArcDistortion);
		MKENUMVAL(PolarDistortion);
		MKENUMVAL(DePolarDistortion);
		MKENUMVAL(BarrelDistortion);
		MKENUMVAL(BarrelInverseDistortion);
		MKENUMVAL(ShepardsDistortion);
		MKENUMVAL(SentinelDistortion);





		//::Magick::PaintMethod;
		MKENUMVAL(PointMethod);
		MKENUMVAL(ReplaceMethod);
		MKENUMVAL(FloodfillMethod);
		MKENUMVAL(FillToBorderMethod);
		MKENUMVAL(ResetMethod);

	}

	//////////////////////////////////////////////////////////////////////////
	Magick::~Magick()
	{

	}

}}

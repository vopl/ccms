cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
INCLUDE(setup.cmake)

PROJECT (entry)




################################
SET(INCLUDE_DB
	../../include/db/sqlite3.hpp
	../../include/db/pg.hpp
)
SOURCE_GROUP(include\\db FILES ${INCLUDE_DB})

SET(SRC_DB
	../../src/db/sqlite3_.cpp
	../../src/db/pg.cpp
)
SOURCE_GROUP(src\\db FILES ${SRC_DB})


################################
SET(INCLUDE_CRYPTO
	../../include/crypto/hash.hpp
	../../include/crypto/rand.hpp
	../../include/crypto/utils.hpp
)
SOURCE_GROUP(include\\crypto FILES ${INCLUDE_CRYPTO})

SET(SRC_CRYPTO
	../../src/crypto/hash.cpp
	../../src/crypto/rand.cpp
	../../src/crypto/utils.cpp
)
SOURCE_GROUP(src\\crypto FILES ${SRC_CRYPTO})



################################
SET(INCLUDE_ROUTER
	../../include/router/contextData.hpp
	../../include/router/contextDataStorage.hpp
	../../include/router/crontab.hpp
	../../include/router/execContext.hpp
	../../include/router/path.hpp
	../../include/router/plugins.hpp
	../../include/router/request.hpp
	../../include/router/router.hpp
	../../include/router/cache.hpp
	../../include/router/log.hpp
	../../include/router/fileContainer.hpp
)
SOURCE_GROUP(include\\router FILES ${INCLUDE_ROUTER})

SET(SRC_ROUTER
	../../src/router/contextData.cpp
	../../src/router/contextDataStorage.cpp
	../../src/router/crontab.cpp
	../../src/router/execContext.cpp
	../../src/router/path.cpp
	../../src/router/plugins.cpp
	../../src/router/request.cpp
	../../src/router/router.cpp
	../../src/router/cache.cpp
	../../src/router/log.cpp
	../../src/router/fileContainer.cpp
)
SOURCE_GROUP(src\\router FILES ${SRC_ROUTER})



################################
SET(INCLUDE_SCRIPTER
	../../include/scripter/jsObject.hpp
	../../include/scripter/newPropsCollector.hpp
	../../include/scripter/scripter.hpp
	../../include/scripter/profiler.hpp
)
SOURCE_GROUP(include\\scripter FILES ${INCLUDE_SCRIPTER})

SET(SRC_SCRIPTER
	../../src/scripter/jsObject.cpp
	../../src/scripter/scripter.cpp
	../../src/scripter/preprocessor.cpp
	../../src/scripter/profiler.cpp
)
SOURCE_GROUP(src\\scripter FILES ${SRC_SCRIPTER})

SET(HDR_SRC_SCRIPTER
	../../src/scripter/preprocessor.hpp
)
SOURCE_GROUP(src\\scripter FILES ${HDR_SRC_SCRIPTER})







################################
SET(INCLUDE_STRUCT
	../../include/struct/filter.hpp
	../../include/struct/filterInstance.hpp
	../../include/struct/point.hpp
	../../include/struct/pointInstance.hpp
	../../include/struct/property.hpp
	../../include/struct/propertyInstance.hpp
	../../include/struct/static.hpp
	../../include/struct/staticInstance.hpp
	../../include/struct/withMaps.hpp
	../../include/struct/withMaps.inc.hpp
)
SOURCE_GROUP(include\\struct FILES ${INCLUDE_STRUCT})

SET(SRC_STRUCT
	../../src/struct/filter.cpp
	../../src/struct/filterInstance.cpp
	../../src/struct/point.cpp
	../../src/struct/pointInstance.cpp
	../../src/struct/property.cpp
	../../src/struct/propertyInstance.cpp
	../../src/struct/static.cpp
	../../src/struct/staticInstance.cpp
)
SOURCE_GROUP(src\\struct FILES ${SRC_STRUCT})




################################
SET(INCLUDE_TRANSPORT
	../../include/transport/transportAsio.hpp
	../../include/transport/transportAsioSocket.hpp
	../../include/transport/transportBase.hpp
)
SOURCE_GROUP(include\\transport FILES ${INCLUDE_TRANSPORT})

SET(SRC_TRANSPORT
	../../src/transport/transportAsio.cpp
	../../src/transport/transportAsioSocket.cpp
	../../src/transport/transportBase.inc.hpp
)
SOURCE_GROUP(src\\transport FILES ${SRC_TRANSPORT})



################################
SET(INCLUDE_I18N
	../../include/i18n/dicTable.hpp
	../../include/i18n/dicTableBucket.hpp
	../../include/i18n/i18n.hpp
	../../include/i18n/l10n.hpp
	../../include/i18n/numberFormatter.hpp
	../../include/i18n/pluralExpression.hpp
	../../include/i18n/pluralExpressionBuilder.hpp
	../../include/i18n/timeFormatter.hpp	
)
SOURCE_GROUP(include\\i18n FILES ${INCLUDE_I18N})

SET(SRC_I18N
	../../src/i18n/dicTable.cpp
	../../src/i18n/dicTableBucket.cpp
	../../src/i18n/i18n.cpp
	../../src/i18n/l10n.cpp
	../../src/i18n/numberFormatter.cpp
	../../src/i18n/timeFormatter.cpp
)
SOURCE_GROUP(src\\i18n FILES ${SRC_I18N})



################################
SET(INCLUDE_UI
	../../include/ui/hookPoint.hpp
	../../include/ui/ui.hpp
	../../include/ui/blockEntry.hpp
)
SOURCE_GROUP(include\\ui FILES ${INCLUDE_UI})

SET(SRC_UI
	../../src/ui/hookPoint.cpp
	../../src/ui/ui.cpp
	../../src/ui/blockEntry.cpp
)
SOURCE_GROUP(src\\ui FILES ${SRC_UI})





################################
SET(INCLUDE_TPM
	../../include/tpm/template.hpp
	../../include/tpm/templateEscaper.hpp
	../../include/tpm/templateSource.hpp
)
SOURCE_GROUP(include\\tpm FILES ${INCLUDE_TPM})

SET(SRC_TPM
	../../src/tpm/template.cpp
	../../src/tpm/templateEscaper.cpp
	../../src/tpm/templateSource.cpp
)
SOURCE_GROUP(src\\tpm FILES ${SRC_TPM})














################################
SET(INCLUDE_MIME
	../../include/mime/content.hpp
	../../include/mime/encoders.hpp
	../../include/mime/file.hpp
	../../include/mime/headerAddress.hpp
	../../include/mime/headerBase.hpp
	../../include/mime/headerComplex.hpp
	../../include/mime/headerDate.hpp
	../../include/mime/headers.hpp
	../../include/mime/headerText.hpp
	../../include/mime/mime.hpp
	../../include/mime/part.hpp
	../../include/mime/utils.hpp
)
SOURCE_GROUP(include\\mime FILES ${INCLUDE_MIME})

SET(SRC_MIME
	../../src/mime/content.cpp
	../../src/mime/encoders.cpp
	../../src/mime/file.cpp
	../../src/mime/headerAddress.cpp
	../../src/mime/headerBase.cpp
	../../src/mime/headerComplex.cpp
	../../src/mime/headerDate.cpp
	../../src/mime/headers.cpp
	../../src/mime/headerText.cpp
	../../src/mime/mime.cpp
	../../src/mime/part.cpp
)
SOURCE_GROUP(src\\mime FILES ${SRC_MIME})














################################
SET(INCLUDE_NET
	../../include/net/net.hpp
	../../include/net/smtp.hpp
)
SOURCE_GROUP(include\\net FILES ${INCLUDE_NET})

SET(SRC_NET
	../../src/net/net.cpp
	../../src/net/smtp.cpp
)
SOURCE_GROUP(src\\net FILES ${SRC_NET})










################################
SET(INCLUDE_UTILS_UTF8
	../../include/utils/utf8/checked.h
	../../include/utils/utf8/unchecked.h
	../../include/utils/utf8/core.h
)
SOURCE_GROUP(include\\utils\\utf8 FILES ${INCLUDE_UTILS_UTF8})

SET(INCLUDE_UTILS
	../../include/utils/base64.h
	../../include/utils/utf8.h
	../../include/utils/crc32.hpp
	../../include/utils/dcvt.h
	../../include/utils/ncvt.h
	../../include/utils/initializable.hpp
	../../include/utils/string.hpp
	../../include/utils/mkgmtime.h
	../../include/utils/httpDate.hpp
	../../include/utils/deflate.hpp
)
SOURCE_GROUP(include\\utils FILES ${INCLUDE_UTILS})


SET(SRC_UTILS
	../../src/utils/base64.cpp
	../../src/utils/crc32.cpp
	../../src/utils/dcvt.cpp
	../../src/utils/mkgmtime.c
	../../src/utils/httpDate.cpp
	../../src/utils/deflate.cpp
)
SOURCE_GROUP(src\\utils FILES ${SRC_UTILS})





################################
SET(INCLUDE
	../../include/assocSeqContainer.hpp
	../../include/exceptions.hpp
	../../include/iTransport.hpp
)
SOURCE_GROUP(include FILES ${INCLUDE})

SET(SRC
	../../src/stdafx.cpp
	../../src/stdafx.h
	../../src/targetver.h
)
SOURCE_GROUP(src FILES ${SRC})





################################
SET(SQLITE
	../../left/sqlite-amalgamation-3_6_23_1/sqlite3.c
	../../left/sqlite-amalgamation-3_6_23_1/sqlite3.h
)
SOURCE_GROUP(left\\sqlite-amalgamation-3_6_23_1 FILES ${SQLITE})









################################
SET(INCLUDE_MAGICK
	../../include/magick/color.hpp
	../../include/magick/core.h
	../../include/magick/drawable.hpp
	../../include/magick/drawableAffine.hpp
	../../include/magick/drawableArc.hpp
	../../include/magick/drawableBase.hpp
	../../include/magick/drawableBezier.hpp
	../../include/magick/drawableCircle.hpp
	../../include/magick/drawableClipPath.hpp
	../../include/magick/drawableColor.hpp
	../../include/magick/drawableCompositeImage.hpp
	../../include/magick/drawableDashArray.hpp
	../../include/magick/drawableDashOffset.hpp
	../../include/magick/drawableEllipse.hpp
	../../include/magick/drawableFillColor.hpp
	../../include/magick/drawableFillOpacity.hpp
	../../include/magick/drawableFillRule.hpp
	../../include/magick/drawableFont.hpp
	../../include/magick/drawableGravity.hpp
	../../include/magick/drawableLine.hpp
	../../include/magick/drawableMatte.hpp
	../../include/magick/drawableMiterLimit.hpp
	../../include/magick/drawablePath.hpp
	../../include/magick/drawablePoint.hpp
	../../include/magick/drawablePointSize.hpp
	../../include/magick/drawablePolygon.hpp
	../../include/magick/drawablePolyline.hpp
	../../include/magick/drawablePopClipPath.hpp
	../../include/magick/drawablePopGraphicContext.hpp
	../../include/magick/drawablePopPattern.hpp
	../../include/magick/drawablePushClipPath.hpp
	../../include/magick/drawablePushGraphicContext.hpp
	../../include/magick/drawablePushPattern.hpp
	../../include/magick/drawableRectangle.hpp
	../../include/magick/drawableRotation.hpp
	../../include/magick/drawableRoundRectangle.hpp
	../../include/magick/drawableScaling.hpp
	../../include/magick/drawableSkewX.hpp
	../../include/magick/drawableSkewY.hpp
	../../include/magick/drawableStrokeAntialias.hpp
	../../include/magick/drawableStrokeColor.hpp
	../../include/magick/drawableStrokeLineCap.hpp
	../../include/magick/drawableStrokeLineJoin.hpp
	../../include/magick/drawableStrokeOpacity.hpp
	../../include/magick/drawableStrokeWidth.hpp
	../../include/magick/drawableText.hpp
	../../include/magick/drawableTextAntialias.hpp
	../../include/magick/drawableTextDecoration.hpp
	../../include/magick/drawableTextUnderColor.hpp
	../../include/magick/drawableTranslation.hpp
	../../include/magick/drawableViewbox.hpp
	../../include/magick/geometry.hpp
	../../include/magick/image.hpp
	../../include/magick/imageAttributes.hpp
	../../include/magick/imageCnvtHelpers.hpp
	../../include/magick/macroHelpers.hpp
	../../include/magick/magick.hpp
	../../include/magick/path.hpp
	../../include/magick/pathArcAbs.hpp
	../../include/magick/pathArcRel.hpp
	../../include/magick/pathBase.hpp
	../../include/magick/pathClosePath.hpp
	../../include/magick/pathCnvtHelpers.hpp
	../../include/magick/pathCurveToAbs.hpp
	../../include/magick/pathCurveToRel.hpp
	../../include/magick/pathLineToAbs.hpp
	../../include/magick/pathLineToHorizontalAbs.hpp
	../../include/magick/pathLineToHorizontalRel.hpp
	../../include/magick/pathLineToRel.hpp
	../../include/magick/pathLineToVerticalAbs.hpp
	../../include/magick/pathLineToVerticalRel.hpp
)
SOURCE_GROUP(include\\magick FILES ${INCLUDE_MAGICK})

SET(SRC_MAGICK
	../../src/magick/color.cpp
	../../src/magick/drawableAffine.cpp
	../../src/magick/drawableArc.cpp
	../../src/magick/drawableBezier.cpp
	../../src/magick/drawableCircle.cpp
	../../src/magick/drawableClipPath.cpp
	../../src/magick/drawableColor.cpp
	../../src/magick/drawableCompositeImage.cpp
	../../src/magick/drawableDashArray.cpp
	../../src/magick/drawableDashOffset.cpp
	../../src/magick/drawableEllipse.cpp
	../../src/magick/drawableFillColor.cpp
	../../src/magick/drawableFillOpacity.cpp
	../../src/magick/drawableFillRule.cpp
	../../src/magick/drawableFont.cpp
	../../src/magick/drawableGravity.cpp
	../../src/magick/drawableLine.cpp
	../../src/magick/drawableMatte.cpp
	../../src/magick/drawableMiterLimit.cpp
	../../src/magick/drawablePath.cpp
	../../src/magick/drawablePoint.cpp
	../../src/magick/drawablePointSize.cpp
	../../src/magick/drawablePolygon.cpp
	../../src/magick/drawablePolyline.cpp
	../../src/magick/drawablePopClipPath.cpp
	../../src/magick/drawablePopGraphicContext.cpp
	../../src/magick/drawablePopPattern.cpp
	../../src/magick/drawablePushClipPath.cpp
	../../src/magick/drawablePushGraphicContext.cpp
	../../src/magick/drawablePushPattern.cpp
	../../src/magick/drawableRectangle.cpp
	../../src/magick/drawableRotation.cpp
	../../src/magick/drawableRoundRectangle.cpp
	../../src/magick/drawableScaling.cpp
	../../src/magick/drawableSkewX.cpp
	../../src/magick/drawableSkewY.cpp
	../../src/magick/drawableStrokeAntialias.cpp
	../../src/magick/drawableStrokeColor.cpp
	../../src/magick/drawableStrokeLineCap.cpp
	../../src/magick/drawableStrokeLineJoin.cpp
	../../src/magick/drawableStrokeOpacity.cpp
	../../src/magick/drawableStrokeWidth.cpp
	../../src/magick/drawableText.cpp
	../../src/magick/drawableTextAntialias.cpp
	../../src/magick/drawableTextDecoration.cpp
	../../src/magick/drawableTextUnderColor.cpp
	../../src/magick/drawableTranslation.cpp
	../../src/magick/drawableViewbox.cpp
	../../src/magick/geometry.cpp
	../../src/magick/image.cpp
	../../src/magick/imageAttributes.cpp
	../../src/magick/magick.cpp
	../../src/magick/pathArcAbs.cpp
	../../src/magick/pathArcRel.cpp
	../../src/magick/pathClosePath.cpp
	../../src/magick/pathCurveToAbs.cpp
	../../src/magick/pathCurveToRel.cpp
	../../src/magick/pathLineToAbs.cpp
	../../src/magick/pathLineToHorizontalAbs.cpp
	../../src/magick/pathLineToHorizontalRel.cpp
	../../src/magick/pathLineToRel.cpp
	../../src/magick/pathLineToVerticalAbs.cpp
	../../src/magick/pathLineToVerticalRel.cpp
	../../src/magick/pathMoveToAbs.cpp
	../../src/magick/pathMoveToRel.cpp
	../../src/magick/pathQuadraticCurveToAbs.cpp
	../../src/magick/pathQuadraticCurveToRel.cpp
	../../src/magick/pathSmoothCurveToAbs.cpp
	../../src/magick/pathSmoothCurveToRel.cpp
	../../src/magick/pathSmoothQuadraticCurveToAbs.cpp
	../../src/magick/pathSmoothQuadraticCurveToRel.cpp
)
SOURCE_GROUP(src\\magick FILES ${SRC_MAGICK})

SET(SRC_MAGICK_PCH
	../../src/magick/magick_pch.h
	../../src/magick/magick_pch.cpp
)
SOURCE_GROUP(src\\magick FILES ${SRC_MAGICK_PCH})


												































################################
SET(ENTRY
	../../src/entry/console.cpp
)
SOURCE_GROUP(src\\entry FILES ${ENTRY})





ADD_EXECUTABLE(entry 
	${INCLUDE_CRYPTO} ${SRC_CRYPTO}
	${INCLUDE_DB} ${SRC_DB}
	${INCLUDE_ROUTER} ${SRC_ROUTER}
	${INCLUDE_SCRIPTER} ${SRC_SCRIPTER} ${HDR_SRC_SCRIPTER}
	${INCLUDE_STRUCT} ${SRC_STRUCT}
	${INCLUDE_TRANSPORT} ${SRC_TRANSPORT}
	${INCLUDE_I18N} ${SRC_I18N}
	${INCLUDE_UI} ${SRC_UI}
	${INCLUDE_TPM} ${SRC_TPM}
	${INCLUDE_UTILS_UTF8} ${INCLUDE_UTILS} ${SRC_UTILS}
	${INCLUDE_MAGICK} ${SRC_MAGICK} ${SRC_MAGICK_PCH}
	${INCLUDE_MIME} ${SRC_MIME}
	${INCLUDE_NET} ${SRC_NET}
	${INCLUDE} ${SRC}
	${SQLITE}
	${ENTRY}
	)

CREATE_PCH(entry ../../src/stdafx.h ../../src/stdafx.cpp)

USE_PCH(entry ../../src/stdafx.h ${SRC_CRYPTO})
USE_PCH(entry ../../src/stdafx.h ${SRC_DB})
USE_PCH(entry ../../src/stdafx.h ${SRC_ROUTER})
USE_PCH(entry ../../src/stdafx.h ${SRC_SCRIPTER})
USE_PCH(entry ../../src/stdafx.h ${SRC_STRUCT})
USE_PCH(entry ../../src/stdafx.h ${SRC_TRANSPORT})
USE_PCH(entry ../../src/stdafx.h ${SRC_I18N})
USE_PCH(entry ../../src/stdafx.h ${SRC_UI})
USE_PCH(entry ../../src/stdafx.h ${SRC_TPM})
USE_PCH(entry ../../src/stdafx.h ${SRC_MIME})
USE_PCH(entry ../../src/stdafx.h ${SRC_NET})
USE_PCH(entry ../../src/stdafx.h ${SRC_UTILS})
USE_PCH(entry ../../src/stdafx.h ${ENTRY})



CREATE_PCH(entry ../../src/magick/magick_pch.h ../../src/magick/magick_pch.cpp)
USE_PCH(entry ../../src/magick/magick_pch.h ${SRC_MAGICK})


IF(MSVC)
	TARGET_LINK_LIBRARIES(entry debug ../../left/js/lib/dbg/js3250 optimized ../../left/js/lib/js3250)
	TARGET_LINK_LIBRARIES(entry libpq)
ELSE(MSVC)
	TARGET_LINK_LIBRARIES(entry mozjs)
	TARGET_LINK_LIBRARIES(entry pq)
	
    TARGET_LINK_LIBRARIES(entry Magick++)
    TARGET_LINK_LIBRARIES(entry MagickCore)
    TARGET_LINK_LIBRARIES(entry boost_system)
    TARGET_LINK_LIBRARIES(entry boost_filesystem)
    TARGET_LINK_LIBRARIES(entry boost_thread)
	
ENDIF(MSVC)


INSTALL(CODE "set(USER ${CMAKE_INSTALL_OWNER})")
INSTALL(CODE "set(PREFIX ${CMAKE_INSTALL_PREFIX})")

install(DIRECTORY ../../cfg DESTINATION .)
install(DIRECTORY ../../ssl DESTINATION .)
install(DIRECTORY ../../scripts DESTINATION .)
install(DIRECTORY ../../www DESTINATION .)
INSTALL(CODE "FILE(MAKE_DIRECTORY \${PREFIX}/bin)")
INSTALL(CODE "FILE(MAKE_DIRECTORY \${PREFIX}/logs)")
INSTALL(CODE "FILE(MAKE_DIRECTORY \${PREFIX}/var)")
INSTALL(CODE "FILE(MAKE_DIRECTORY \${PREFIX}/www/filestore)")
INSTALL(CODE "FILE(MAKE_DIRECTORY \${PREFIX}/filestore)")


INSTALL(TARGETS entry RUNTIME DESTINATION bin)

INSTALL(CODE "configure_file(../../bin/ctl.sh.sample \${PREFIX}/bin/ctl.sh)")
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink \${PREFIX}/bin/ctl.sh /etc/init.d/ccms)")


set(mainCfg "${CMAKE_INSTALL_PREFIX}/cfg/main.js")
if (EXISTS ${mainCfg})
	install(CODE "message(\"found old config ${mainCfg}, keep\")")
else(EXISTS ${mainCfg})
	install(CODE "message(\" not found config ${mainCfg}, create\")")
	INSTALL(FILES ../../cfg/main.js.sample DESTINATION cfg RENAME main.js)
endif(EXISTS ${mainCfg})
	
INSTALL(CODE
	"EXECUTE_PROCESS(
		COMMAND chown -R ${CMAKE_INSTALL_OWNER} ${CMAKE_INSTALL_PREFIX}
		COMMAND chgrp -R ${CMAKE_INSTALL_GROUP} ${CMAKE_INSTALL_PREFIX}
		COMMAND chmod -R ug=rwX ${CMAKE_INSTALL_PREFIX}
		COMMAND chown root ${CMAKE_INSTALL_PREFIX}/bin/ctl.sh
		COMMAND chgrp root ${CMAKE_INSTALL_PREFIX}/bin/ctl.sh
		COMMAND chmod u=rwx ${CMAKE_INSTALL_PREFIX}/bin/ctl.sh
	)"
)



INCLUDEPATH+=$$S_GFW_DIR
include($$S_GFW_DIR/gArray/gArray.pri)
include($$S_GFW_DIR/gBase/gBase.pri)
include($$S_GFW_DIR/gDsp/gDsp.pri)
include($$S_GFW_DIR/gVariant/gVariant.pri)
include($$S_GFW_DIR/gFile/gFile.pri)
include($$S_GFW_DIR/gLexer/gLexer.pri)
include($$S_GFW_DIR/gLibrary/gLibrary.pri)
include($$S_GFW_DIR/gLinkedList/gLinkedList.pri)
include($$S_GFW_DIR/gHashMap/gHashMap.pri)
include($$S_GFW_DIR/gMemory/gMemory.pri)
include($$S_GFW_DIR/gRect/gRect.pri)
include($$S_GFW_DIR/gsCoder/gsCoder.pri)
include($$S_GFW_DIR/gString/gString.pri)
include($$S_GFW_DIR/gTree/gTree.pri)
GFW_GUDA{
include($$S_GFW_DIR/gUda/gUda.pri)
}
include($$S_GFW_DIR/gVirtualMemory/gVirtualMemory.pri)
include($$S_GFW_DIR/gVm/gVm.pri)
include($$S_GFW_DIR/gVm/gVmAsm.pri)
include($$S_GFW_DIR/gXMLParser/gXMLParser.pri)
include($$S_GFW_DIR/gCsl/gCsl.pri)
include($$S_GFW_DIR/gMath/gMath.pri)
include($$S_GFW_DIR/gTimers/gTimers.pri)
include($$S_GFW_DIR/gTime/gTime.pri)
include($$S_GFW_DIR/gThreads/gThreads.pri)
include($$S_GFW_DIR/gImage/gImage.pri)
include($$S_GFW_DIR/gBinPacker/gBinPacker.pri)
include($$S_GFW_DIR/gAlignedLists/gAlignedLists.pri)
include($$S_GFW_DIR/gUrl/gUrl.pri)
include($$S_GFW_DIR/gTable/gTables.pri)
unix:include($$S_GFW_DIR/gCom/gCom.pri)
HEADERS+=$$S_GFW_DIR/gfw_global.h \
         $$S_GFW_DIR/gfw.h \
    gfw_export.h

OTHER_FILES += \
    gfw_workflow.txt \
    gfw_notes.txt

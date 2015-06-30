#############################################################################
##
## Copyright (C) 2013 iCNTV Corporation and/or its subsidiary(-ies).
## All rights reserved.
##
## This file is part of the iCNTV Platform Terminal System.
##
## This confidential and proprietary software may be used only as
## authorised by a licensing agreement from iCNTV Limited
##
## The entire notice above must be reproduced on all authorised copies
## and copies may only be made to the extent permitted by a licensing
## agreement from iCNTV Limited.
##
## Contact :  Jiang Lu (jiang.lu@icntv.tv)
##
#############################################################################
OLD_LOCAL_PATH := $(LOCAL_PATH)

###########################################################
# libpoco_xml.a
###########################################################
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := poco_xml

LOCAL_CFLAGS += -fvisibility=hidden
LOCAL_CFLAGS += -DPOCO_ANDROID \
                -DPOCO_NO_FPENVIRONMENT \
                -DPOCO_NO_WSTRING \
                -DPOCO_NO_SHAREDMEMORY \
                -DHAVE_EXPAT_CONFIG_H \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_C_INCLUDES := $(LOCAL_EXPORT_C_INCLUDES)

LOCAL_SRC_FILES := \
    src/AbstractContainerNode.cpp \
    src/AbstractNode.cpp \
    src/Attr.cpp \
    src/AttrMap.cpp \
    src/Attributes.cpp \
    src/AttributesImpl.cpp \
    src/CDATASection.cpp \
    src/CharacterData.cpp \
    src/ChildNodesList.cpp \
    src/Comment.cpp \
    src/ContentHandler.cpp \
    src/DOMBuilder.cpp \
    src/DOMException.cpp \
    src/DOMImplementation.cpp \
    src/DOMObject.cpp \
    src/DOMParser.cpp \
    src/DOMSerializer.cpp \
    src/DOMWriter.cpp \
    src/DTDHandler.cpp \
    src/DTDMap.cpp \
    src/DeclHandler.cpp \
    src/DefaultHandler.cpp \
    src/Document.cpp \
    src/DocumentEvent.cpp \
    src/DocumentFragment.cpp \
    src/DocumentType.cpp \
    src/Element.cpp \
    src/ElementsByTagNameList.cpp \
    src/Entity.cpp \
    src/EntityReference.cpp \
    src/EntityResolver.cpp \
    src/EntityResolverImpl.cpp \
    src/ErrorHandler.cpp \
    src/Event.cpp \
    src/EventDispatcher.cpp \
    src/EventException.cpp \
    src/EventListener.cpp \
    src/EventTarget.cpp \
    src/InputSource.cpp \
    src/LexicalHandler.cpp \
    src/Locator.cpp \
    src/LocatorImpl.cpp \
    src/MutationEvent.cpp \
    src/Name.cpp \
    src/NamePool.cpp \
    src/NamedNodeMap.cpp \
    src/NamespaceStrategy.cpp \
    src/NamespaceSupport.cpp \
    src/Node.cpp \
    src/NodeAppender.cpp \
    src/NodeFilter.cpp \
    src/NodeIterator.cpp \
    src/NodeList.cpp \
    src/Notation.cpp \
    src/ParserEngine.cpp \
    src/ProcessingInstruction.cpp \
    src/SAXException.cpp \
    src/SAXParser.cpp \
    src/Text.cpp \
    src/TreeWalker.cpp \
    src/WhitespaceFilter.cpp \
    src/XMLException.cpp \
    src/XMLFilter.cpp \
    src/XMLFilterImpl.cpp \
    src/XMLReader.cpp \
    src/XMLString.cpp \
    src/XMLWriter.cpp \
    src/xmlparse.cpp \
    src/xmlrole.c \
    src/xmltok.c \
    src/xmltok_impl.c \
    src/xmltok_ns.c \

#LOCAL_LDLIBS := -lstdc++ -lsupc++

LOCAL_STATIC_LIBRARIES := poco_foundation

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)

###########################################################
$(call import-module, thirdparty/poco/foundation)


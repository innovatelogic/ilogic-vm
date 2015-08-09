#ifndef __assetbrowserframe_h__
#define __assetbrowserframe_h__

#pragma once

#include <commctrl.h>
#include <string>

struct SNode
{
	std::wstring sName;
	unsigned long fAttribute;

	SNode(const std::wstring &name, unsigned int attr = 0)
		: sName(name)
		, fAttribute(attr)
	{}
};


class IAssetBrowserFrame
{
public:
	virtual ~IAssetBrowserFrame() {}

	virtual void OnFolderSelected(SNode *pNodeParent) = 0;

	virtual class CCoreSDK* GetAppMain() const = 0;

	virtual void SetOpenFileName(const wchar_t *strName) = 0;

	virtual HIMAGELIST GetImageList() const = 0;
};

#endif//__assetbrowserframe_h__
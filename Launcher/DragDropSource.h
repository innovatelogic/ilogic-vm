#ifndef __dragdropsource_h__
#define __dragdropsource_h__

#pragma once

#include "atlbase.h"
#include "atlcom.h"

extern std::wstring g_DDFilename;

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CDragDropSource :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDragDropSource>,
	public IDataObject,
	public IDropSource
{
	// This typedef creates an IEnumFORMATETC enumerator that the drag source
	// uses in EnumFormatEtc().
	typedef CComEnumOnSTL<  IEnumFORMATETC,           // name of enumerator interface
							&IID_IEnumFORMATETC,      // IID of enumerator interface
							FORMATETC,                // type of object to return
							_Copy<FORMATETC>,         // copy policy class
							std::vector<FORMATETC> >  // type of collection holding the data
							CEnumFORMATETCImpl;

	typedef struct 
	{
		FORMATETC fe;
		STGMEDIUM stgm;
	} DATAENTRY, *LPDATAENTRY;

public:
	// Maps
	BEGIN_COM_MAP(CDragDropSource)
		COM_INTERFACE_ENTRY(IDataObject)
		COM_INTERFACE_ENTRY(IDropSource)
	END_COM_MAP()

	//----------------------------------------------------------------------------------------------
	// Construction
	CDragDropSource()
		: m_bInitialized(false)
		, m_dwLastEffect(DROPEFFECT_NONE)
		, m_cde(0) 
		, m_rgde(nullptr)
	{
	}

	//----------------------------------------------------------------------------------------------
	CDragDropSource::~CDragDropSource()
	{
		for (int i = 0; i < m_cde; i++)
		{
			CoTaskMemFree(m_rgde[i].fe.ptd);
			ReleaseStgMedium(&m_rgde[i].stgm);
		}
		CoTaskMemFree(m_rgde);
	}

	//----------------------------------------------------------------------------------------------
	bool Init(const wchar_t *filepath)
	{
		FORMATETC fetc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		STGMEDIUM stg = { TYMED_HGLOBAL };
		size_t    cbyData = 0;
		HGLOBAL   hgl;

		m_filename = filepath;

		cbyData += sizeof(TCHAR) * (1 + m_filename.size());

		// Alloc a buffer to hold data.
		hgl = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, cbyData);

		if (NULL == hgl){
			return false;
		}

		DROPFILES* pDrop = (DROPFILES*) GlobalLock(hgl);

		if (NULL == pDrop)
		{
			GlobalFree(hgl);
			return false;
		}

		pDrop->pFiles = sizeof(DROPFILES);
		pDrop->fWide = 1; // _UNICODE

		// Copy the filenames into the buffer.
		LPTSTR pszFilename = (LPTSTR) (pDrop + 1);

		g_DDFilename = m_filename;

		GlobalUnlock(hgl);
		stg.hGlobal = hgl;

		if (FAILED(SetData(&fetc, &stg, TRUE)))
		{
			GlobalFree ( hgl );
			return false;
		}

		m_bInitialized = true;
		return true;
	}

	//----------------------------------------------------------------------------------------------
	HRESULT DoDragDrop(DWORD dwOKEffects, DWORD *pdwEffect)
	{
		if (!m_bInitialized){
			return E_FAIL;
		}

		CComQIPtr<IDropSource> pDropSrc = this;
		CComQIPtr<IDataObject> pDataObj = this;

		ATLASSERT(pDropSrc && pDataObj);

		return ::DoDragDrop(pDataObj, pDropSrc, dwOKEffects, pdwEffect);
	}

	//----------------------------------------------------------------------------------------------
	// IDataObject
	STDMETHODIMP SetData(FORMATETC *pfe, STGMEDIUM *pstgm, BOOL fRelease)
	{
		if (!fRelease){
			return E_NOTIMPL;
		}
		
		LPDATAENTRY pde;
		HRESULT hres = FindFORMATETC(pfe, &pde, TRUE);

		if (SUCCEEDED(hres))
		{
			if (pde->stgm.tymed)
			{
				ReleaseStgMedium(&pde->stgm);
				ZeroMemory(&pde->stgm, sizeof(STGMEDIUM));
			}

			if (fRelease) 
			{
				pde->stgm = *pstgm;
				hres = S_OK;
			}
			else 
			{
				hres = AddRefStgMedium ( pstgm, &pde->stgm, TRUE );
			}

			pde->fe.tymed = pde->stgm.tymed; 

			if (GetCanonicalIUnknown(pde->stgm.pUnkForRelease ) == GetCanonicalIUnknown(static_cast<IDataObject*>(this)))
			{
				pde->stgm.pUnkForRelease->Release();
				pde->stgm.pUnkForRelease = NULL;
			}
		}

		return hres;
	}

	//----------------------------------------------------------------------------------------------
	STDMETHODIMP GetData(FORMATETC *pformatetcIn, STGMEDIUM *pmedium)
	{
		return S_OK;
	}

	//----------------------------------------------------------------------------------------------
	STDMETHODIMP EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC **ppenumFormatEtc)
	{
		HRESULT hr;
		CComObject<CEnumFORMATETCImpl> *pImpl;

		// Create an enumerator object.
		hr = CComObject<CEnumFORMATETCImpl>::CreateInstance(&pImpl);

		if (FAILED(hr)){
			return hr;
		}

		pImpl->AddRef();

		// Fill in m_vecSupportedFormats with the formats that the caller has
		// put in this object.
		m_vecSupportedFormats.clear();

		for (int i = 0; i < m_cde; i++){
			m_vecSupportedFormats.push_back(m_rgde[i].fe);
		}

		// Init the enumerator, passing it our vector of supported FORMATETCs.
		hr = pImpl->Init(GetUnknown(), m_vecSupportedFormats);

		if (FAILED(hr))
		{
			pImpl->Release();
			return E_UNEXPECTED;
		}

		// Return the requested interface to the caller.
		hr = pImpl->QueryInterface(ppenumFormatEtc);

		pImpl->Release();
		return hr;
	}

	//----------------------------------------------------------------------------------------------
	HRESULT FindFORMATETC(FORMATETC* pfe, LPDATAENTRY *ppde, BOOL fAdd)
	{
		*ppde = NULL;

		/* Comparing two DVTARGETDEVICE structures is hard, so we don't even try */
		if (pfe->ptd != NULL){
			return DV_E_DVTARGETDEVICE;
		}

		/* See if it's in our list */
		int ide = 0;
		for (; ide < m_cde; ide++)
		{
			if (m_rgde[ide].fe.cfFormat == pfe->cfFormat &&
				m_rgde[ide].fe.dwAspect == pfe->dwAspect &&
				m_rgde[ide].fe.lindex == pfe->lindex )
			{
				if (fAdd || ( m_rgde[ide].fe.tymed & pfe->tymed))
				{
					*ppde = &m_rgde[ide];
					return S_OK;
				}
				else 
				{
					return DV_E_TYMED;
				}
			}
		}

		if (!fAdd){
			return DV_E_FORMATETC;
		}

		LPDATAENTRY pdeT = (LPDATAENTRY) CoTaskMemRealloc(m_rgde, sizeof(DATAENTRY) * (m_cde + 1));

		if (pdeT)
		{
			m_rgde = pdeT;
			m_cde++;
			m_rgde[ide].fe = *pfe;

			ZeroMemory(&pdeT[ide].stgm, sizeof(STGMEDIUM));
			*ppde = &m_rgde[ide];

			return S_OK;
		} 
		else 
		{
			return E_OUTOFMEMORY;
		}
	}

	//----------------------------------------------------------------------------------------------
	HRESULT AddRefStgMedium(STGMEDIUM* pstgmIn, STGMEDIUM* pstgmOut, BOOL fCopyIn)
	{
		HRESULT hres = S_OK;
		STGMEDIUM stgmOut = *pstgmIn;

		if (pstgmIn->pUnkForRelease == NULL && !(pstgmIn->tymed & (TYMED_ISTREAM | TYMED_ISTORAGE))) 
		{
			if ( fCopyIn )
			{
				/* Object needs to be cloned */
				if ( pstgmIn->tymed == TYMED_HGLOBAL )
				{
					stgmOut.hGlobal = GlobalClone( pstgmIn->hGlobal );

					if (!stgmOut.hGlobal)
						hres = E_OUTOFMEMORY;
				}
				else {
					hres = DV_E_TYMED;      /* Don't know how to clone GDI objects */
				}
			} 
			else  {
				stgmOut.pUnkForRelease = static_cast<IDataObject*>(this);
			}
		}

		if (SUCCEEDED(hres))
		{
			switch (stgmOut.tymed) 
			{
			case TYMED_ISTREAM:
				stgmOut.pstm->AddRef();
				break;

			case TYMED_ISTORAGE:
				stgmOut.pstg->AddRef();
				break;
			}

			if (stgmOut.pUnkForRelease){
				stgmOut.pUnkForRelease->AddRef();
			}

			*pstgmOut = stgmOut;
		}

		return hres;
	}

	//----------------------------------------------------------------------------------------------
	HGLOBAL GlobalClone(HGLOBAL hglobIn)
	{
		HGLOBAL hglobOut = NULL;
		LPVOID pvIn = GlobalLock(hglobIn);

		if (pvIn)
		{
			SIZE_T cb = GlobalSize(hglobIn);

			HGLOBAL hglobOut = GlobalAlloc(GMEM_FIXED, cb);

			if (hglobOut){
				CopyMemory(hglobOut, pvIn, cb);
			}

			GlobalUnlock(hglobIn);
		}

		return hglobOut;
	}

	//----------------------------------------------------------------------------------------------
	IUnknown* GetCanonicalIUnknown(IUnknown* punk)
	{
		IUnknown *punkCanonical;

		if (punk && SUCCEEDED(punk->QueryInterface(IID_IUnknown, (void**) &punkCanonical)))
		{
			punkCanonical->Release();
		}
		else 
		{
			punkCanonical = punk;
		}

		return punkCanonical;
	}

	//----------------------------------------------------------------------------------------------
	STDMETHODIMP QueryGetData(FORMATETC *pformatetc)
	{
		return S_OK;
	}

	//----------------------------------------------------------------------------------------------
	// IDropSource
	STDMETHODIMP QueryContinueDrag(BOOL fEscapePressed, DWORD grfKeyState)
	{
		if (fEscapePressed){
			return DRAGDROP_S_CANCEL;
		}
		else if (!(grfKeyState & MK_LBUTTON))
		{
			// If the last DROPEFFECT we got in GiveFeedback()
			// was DROPEFFECT_NONE, we abort because the allowable
			// effects of the source and target don't match up.
			if (DROPEFFECT_NONE == m_dwLastEffect){
				return DRAGDROP_S_CANCEL;
			}
			// TODO: Extract files from the CAB here...

			//if ( ExtractFilesFromCab() || m_bAbortingDueToMissingCab )
			//	return DRAGDROP_S_DROP;
			//else
				return E_UNEXPECTED;
		}
		else
			return S_OK;
	}

	//----------------------------------------------------------------------------------------------
	STDMETHODIMP GiveFeedback(DWORD dwEffect)
	{
		m_dwLastEffect = dwEffect;
		return DRAGDROP_S_USEDEFAULTCURSORS;
	}

	//----------------------------------------------------------------------------------------------
	STDMETHODIMP GetDataHere(FORMATETC *pformatetc, STGMEDIUM *pmedium)	{ return E_NOTIMPL; }
	STDMETHODIMP GetCanonicalFormatEtc(FORMATETC *pformatectIn, FORMATETC* pformatetcOut) { return E_NOTIMPL; }
	STDMETHODIMP DAdvise(FORMATETC *pformatetc, DWORD advf, IAdviseSink* pAdvSink, DWORD *pdwConnection) { return E_NOTIMPL; }
	STDMETHODIMP DUnadvise(DWORD dwConnection) { return E_NOTIMPL; }
	STDMETHODIMP EnumDAdvise(IEnumSTATDATA **ppenumAdvise) { return E_NOTIMPL; }


private:
	bool  m_bInitialized;
	DWORD m_dwLastEffect;

	LPDATAENTRY m_rgde;                 // Array of active DATAENTRY entries
	int m_cde;                          // Size of m_rgde

	// List of FORMATETCs for which we have data, used in EnumFormatEtc
	std::vector<FORMATETC> m_vecSupportedFormats;

	std::wstring m_filename;
};

#endif//__dragdropsource_h__
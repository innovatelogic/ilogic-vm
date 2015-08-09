#include "m_file.h"

#ifdef WIN32
#pragma warning (disable:4996) 
#endif

//------------------------------------------------------------------------
m_file::m_file():_sFilename("")
,_hFile(0) 
,_eMode(m_file::CLOSED_MODE)
{

}

//------------------------------------------------------------------------
m_file::m_file(const char *sFileName, teMode eMode):_sFilename("")
,_hFile(0) 
,_eMode(eMode)
{
	if(_eMode != CLOSED_MODE)
	{
		_hFile = fopen(_sFilename.c_str(),ModeString(_eMode));
		assert(_hFile!=0);
	}
}
//------------------------------------------------------------------------
m_file::~m_file()
{
	if(_eMode != CLOSED_MODE)
	{
		int nError;

		nError = fflush(_hFile);
		assert(nError!=0);

		nError = fclose(_hFile);
		assert(nError!=0);
	}
}

//------------------------------------------------------------------------
std::string m_file::filename() const
{
	return _sFilename;
}
//------------------------------------------------------------------------
m_file::teMode m_file::mode() const
{
	return _eMode;
}
//------------------------------------------------------------------------
m_file::teError m_file::open(const char * sFilename, teMode eMode)
{
	if ( _eMode!=CLOSED_MODE )
	{
		int nError;

		nError = fflush(_hFile);
		if(nError!=0)
			return UNKNOWN_FILE_ERROR;

		nError = fclose(_hFile);
		if(nError!=0)
			return UNKNOWN_FILE_ERROR;
	}

	_eMode = eMode;
	_sFilename = sFilename;

	if (_eMode != CLOSED_MODE)
	{
		_hFile = fopen(_sFilename.c_str(), ModeString(_eMode));
		if(0 == _hFile)
			return UNKNOWN_FILE_ERROR;
	}

	return NO_FILE_ERROR;
}
//------------------------------------------------------------------------
/// Close this file.
m_file::teError m_file::close()
{
	if (CLOSED_MODE != _eMode)
	{
		int nError;

		nError = fclose(_hFile);
		if(0 != nError)
			return UNKNOWN_FILE_ERROR;
		_eMode = CLOSED_MODE;
	}

	return NO_FILE_ERROR;
}
//------------------------------------------------------------------------
/// Reopen the current file.
m_file::teError m_file::reopen(teMode eMode)
{
	assert(CLOSED_MODE == _eMode);

	_hFile = fopen(_sFilename.c_str(), ModeString(eMode));
	if(0 == _hFile)
		return UNKNOWN_FILE_ERROR;
	_eMode = eMode;

	return NO_FILE_ERROR;
}
//------------------------------------------------------------------------
/// Read data.
int m_file::read(void * pData, int nBytes)
{
	assert(READ_MODE == _eMode);
	return (int)fread(pData, 1, nBytes, _hFile);
}
//------------------------------------------------------------------------
/// Write data.
int m_file::write(const void * pData, int nBytes)
{
	assert(WRITE_MODE == _eMode);
	return (int)fwrite(pData, 1, nBytes, _hFile);
}
//------------------------------------------------------------------------
const char* m_file::ModeString(m_file::teMode eMode)
{
	switch(eMode)
	{
	case READ_MODE:
		return "rb";
	case WRITE_MODE:
		return "wb";
	default:
		return "";
	}
}
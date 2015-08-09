#ifndef _m_file_h_
#define _m_file_h_

#ifdef _WIN32
 #pragma once
#endif

#include "m_streams.h"

class EXPORT m_file : public m_input_output_stream
{
public:
	enum teMode
	{
		READ_MODE = 0,
		WRITE_MODE,
		CLOSED_MODE
	};

	enum teError
	{
		NO_FILE_ERROR = 0,
		READ_ONLY_FILE_ERROR,
		UNKNOWN_FILE_ERROR
	};

	m_file();
	m_file(const char *sFileName, teMode eMode = READ_MODE);
	virtual ~m_file();

	/// Get the current file's filename.
	std::string filename()const;


	/// Get the current file's mode.
	teMode  mode()const;
	teError open(const char * sFilename, teMode eMode = READ_MODE);
	teError reopen(teMode eMode = READ_MODE);
	teError close();

	int read(void * pData, int nBytes);
	int write(const void * pData, int nBytes);

private: 
	static const char* ModeString(m_file::teMode eMode);

	std::string _sFilename;
	FILE*       _hFile;
	teMode      _eMode;
};

#endif
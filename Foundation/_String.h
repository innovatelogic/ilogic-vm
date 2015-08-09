#ifndef __string_h__
#define __string_h__

#pragma once 

#include "platform_specific.h"


class EXPORT String
{
public:
   //Constructors/Destructor
   String();
   String(const char *string);
   String(const String &str);
  ~String();

  bool        Set(const char *string);                    // Assign a string
  bool        Set(const String &string);                  // Assign a string
  inline_     char* Get()       const { return mText; }
  inline_     char* GetBuffer() const { return mText; }

  void        Reset();

  udword      Length() const;                             // Get the string's length
  inline_     udword  GetLength() const { return Length(); }

  sdword      Compare(const String &str2)         const;  // Compare the String with another one
  sdword      Compare(const char *str2)           const;  // Compare the String with another one
//udword      FuzzyCompare(const String& str2)    const;  // Fuzzy comparisons between strings

  String&     ToUpper();                                  // Transform into uppercase
  String&     ToLower();                                  // Transform into lowercase
  // Eval
//   bool        IsDecimal(sdword* value=null)       const;
//   bool        IsFloat(float* value=null)          const;
//   bool        IsFile()                            const;
//   bool        IsBool()                            const;
//   sdword      ToDecimal()                         const;
//   float       ToFloat()                           const;

//   udword      Replace(char oldchar, char newchar);        // Replace all 'oldchar' characters with 'newchar' ones
//   // Parameters
//   bool        GetParameter(String& param, char*& curpar)  const;  // Parameters parsing
//   udword      NbParams()  const;
// 
//   String&     SetAt(ubyte cara, udword pos);
//   ubyte       GetAt(udword pos);

//   // String extraction methods
//   String      Mid(udword first, udword count = (udword)-1)    const;
//   String      Left(udword count)                      const;
//   String      Right(udword count)                     const;
// 
//   // Searching methods (to search a specific character or a sub-String)
//   sdword      Find(ubyte character, udword from = 0, sdword to = -1);
//   sdword      Find(const String& stringtofind, udword from = 0, sdword to = -1);
//   sdword      ReverseFind(ubyte character);
// 
//   // Numerical operand extraction methods
//   bool        Extract(float& f, udword from);
//   bool        Extract(udword& i, udword from);
//   bool        Extract(sdword& i, udword from);

  // Plain-text methods (where starts the next line? where is the end of the current line? etc...)
//   sdword      NextLine(sdword from);
//   sdword      GetEndOfLine(sdword from);

  inline_         bool        IsValid()                           const   { return mText != 0; }
  inline_         bool        IsEmpty()                           const   { return Length() == 0; }
  // Cast operators
  //! Cast operator for const char* = String
  inline_         operator const char*()                          const   { return (const char*)(mText); }
  //! Cast operator for char* = String
  inline_         operator char*()                                const   { return (char*)(mText); }
  //! Cast operator for float = String
//  inline_         operator float()                                const   { float tmp = MAX_FLOAT; IsFloat(&tmp); return tmp;         }
  //! Cast operator for sdword = String
//  inline_         operator sdword()                               const   { sdword tmp = INVALID_NUMBER; IsDecimal(&tmp); return tmp; }

  // Access operators
  const char  operator[](udword i)                const   { return mText ? mText[i] : 0;                              }

  // Assignment operators
  //! Operator for String = const char*
  inline_         String&     operator=(const char* string)               { Set(string);              return *this; }
  //! Operator for String = char*
  inline_         String&     operator=(char* string)                     { Set((const char*)string); return *this; }
  //! Operator for String = String
  inline_         String&     operator=(const String& string)             { Set(string);              return *this; }

  //! Concatenation operator for String += String
//  String&     operator+=(const String& string);
  // Stats

  char*       mText;          //!< The character string held in the class
protected:
private:
};

#endif //__string_h__
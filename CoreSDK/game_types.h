#pragma once

#ifndef CORESDK_EXPORTS
#define CORESDK_API __declspec(dllexport)
#else
#define CORESDK_API __declspec(dllimport)
#endif
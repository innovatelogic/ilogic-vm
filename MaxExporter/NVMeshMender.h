#ifndef _NVMeshMender_H_
#define _NVMeshMender_H_

#pragma once

#include "platform_specific.h"
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////////////////
class EXPORT NVMeshMender
{
    private :
        mutable std::vector<std::string> LastErrors_;

		struct Edge
		{
			unsigned int v0;
			unsigned int v1;

			unsigned int face;
			unsigned int face2;

			bool operator==( const Edge& rhs ) const
			{
                return ( ( v0 == rhs.v0 ) && ( v1 == rhs.v1 ) );
			}

			bool operator<( const Edge& rhs ) const
			{
                if ( v0 < rhs.v0 ) 
                {
                    return true;
                }

                if ( v0 > rhs.v0 )
                {
                    return false;
                }

                return  ( v1 < rhs.v1 );
			}
		};

    public :

        void SetLastError( const std::string& rhs ) const
        {
            LastErrors_.push_back( rhs );
        }

        std::string GetLastError() const
        {
            std::string aString;

            if ( LastErrors_.size() > 0 )
            {
                aString = LastErrors_.back();
            }
            return aString;
        }

        struct VertexAttribute
        {
            std::string  Name_;

            typedef std::vector< int > IntVector;
            IntVector intVector_;


            typedef std::vector< float > FloatVector;
            FloatVector floatVector_;

            VertexAttribute& operator=( const VertexAttribute& rhs )
            {
                Name_   = rhs.Name_;
                intVector_ = rhs.intVector_;
                floatVector_ = rhs.floatVector_;
                return *this;
            }

            VertexAttribute( const char* pName = "" ) : Name_(pName) {;}

            VertexAttribute( const VertexAttribute& rhs )
            {
                *this = rhs;
            }

            bool operator==( const VertexAttribute& rhs )
            {
                return ( Name_ == rhs.Name_ );
            }

            bool operator<( const VertexAttribute& rhs )
            {
                return ( Name_ < rhs.Name_ );
            }

        };

        typedef std::vector< VertexAttribute > VAVector;

		enum Option
		{
			FixTangents,
			DontFixTangents,

			FixCylindricalTexGen,
			DontFixCylindricalTexGen,

            WeightNormalsByFaceSize,
            DontWeightNormalsByFaceSize
		};

        bool NVMeshMender::Munge( const NVMeshMender::VAVector& input, 
			                   NVMeshMender::VAVector& output, 
							   const float bSmoothCreaseAngleRadians = 3.141592654f / 3.0f,
							   const float* pTextureMatrix = 0,
							   const Option _FixTangents = FixTangents,
							   const Option _FixCylindricalTexGen = FixCylindricalTexGen,
                               const Option _WeightNormalsByFaceSize = WeightNormalsByFaceSize
							   );
		bool NVMeshMender::MungeD3DX( const NVMeshMender::VAVector& input, 
			                   NVMeshMender::VAVector& output, 
							   const float bSmoothCreaseAngleRadians = 3.141592654f / 3.0f,
							   const float* pTextureMatrix = 0,
							   const Option _FixTangents = FixTangents,
							   const Option _FixCylindricalTexGen = FixCylindricalTexGen,
                               const Option _WeightNormalsByFaceSize = WeightNormalsByFaceSize
							   );
};

#endif  //_NVMeshMender_H_
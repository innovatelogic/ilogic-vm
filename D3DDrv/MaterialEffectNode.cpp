#include "d3ddriverstdafx.h"

namespace oes
{
    namespace d3d
    {
        //----------------------------------------------------------------------------------------------
        // MaterialEffectNode
        //----------------------------------------------------------------------------------------------
        MaterialEffectNode::MaterialEffectNode(const D3DDriver *pDriver)
            : m_ShaderProgram(0)
            , m_p3DDriver(const_cast<D3DDriver*>(pDriver))
        {
            m_pRenderContext = m_p3DDriver->GetCurrentContext();
        }

        //----------------------------------------------------------------------------------------------
        MaterialEffectNode::~MaterialEffectNode()
        {

        }

        //----------------------------------------------------------------------------------------------
        void MaterialEffectNode::DoRelease()
        {
            glUseProgram(0);
            glDeleteProgram(m_ShaderProgram);

            Refcount::DoRelease();
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectNode::CompileEffect(const char *FilePath)
        {
            bool bResult = true;

            std::ifstream file(FilePath);

            if (file)
            {
                std::string line;
                std::vector<std::string> lines;

                bool grabParams = false;
                while (getline(file, line))
                {
                    if (!grabParams && line == std::string("//> VARS")) {
                        grabParams = true;
                        continue;
                    }
                    if (line == std::string("//> END_VARS")) {
                        break;
                    }
                    if (grabParams && line != "") {
                        lines.push_back(line);
                    }
                }

                for (std::vector<std::string>::iterator Iter = lines.begin(); Iter != lines.end(); ++Iter)
                {
                    std::string SourceString = *Iter + " ";
                    std::vector<std::string> siblin_lines;

                    // brake by spaces
                    while (SourceString.find(" ") != std::string::npos)
                    {
                        size_t BrakeIndex = SourceString.find(" ");

                        if (BrakeIndex > 0)
                        {
                            std::string Value = SourceString.substr(0, BrakeIndex);

                            std::string::size_type n = 0;
                            while ((n = Value.find(":", n)) != std::string::npos) {
                                Value.erase(n, 1);
                            }

                            n = 0;
                            while ((n = Value.find(";", n)) != std::string::npos) {
                                Value.erase(n, 1);
                            }

                            n = 0;
                            while ((n = Value.find("\t", n)) != std::string::npos) {
                                Value.erase(n, 1);
                            }

                            n = 0;
                            while ((n = Value.find("[", n)) != std::string::npos) {
                                Value.erase(n);
                            }

                            if (Value != "") {
                                siblin_lines.push_back(Value);
                            }
                        }
                        SourceString = SourceString.substr(BrakeIndex + 1);
                    }

                    // switch tokens
                    if (siblin_lines.size() >= 2)
                    {
                        if (siblin_lines[0] == "bool") {
                            Params_bool.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float4x4") {
                            Params_float4x4.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float4x3") {
                            Params_float4x3.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float3x3") {
                            Params_float3x3.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float4") {
                            Params_float4.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float3") {
                            Params_float3.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float") {
                            Params_float.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "texture") {
                            Params_texture.push_back(siblin_lines[1]);
                        }
                    }
                }

                bResult = true;
                file.close();
            }
            return bResult;
        }

        //----------------------------------------------------------------------------------------------
        bool MaterialEffectNode::CompileEffectW(const wchar_t *FilePath)
        {
            bool bResult = true;

            std::ifstream file(FilePath);

            if (file)
            {
                std::string line;
                std::vector<std::string> lines;

                bool grabParams = false;
                while (getline(file, line))
                {
                    if (!grabParams && line == std::string("//> VARS")) {
                        grabParams = true;
                        continue;
                    }
                    if (line == std::string("//> END_VARS")) {
                        break;
                    }
                    if (grabParams && line != "") {
                        lines.push_back(line);
                    }
                }

                for (std::vector<std::string>::iterator Iter = lines.begin(); Iter != lines.end(); ++Iter)
                {
                    std::string SourceString = *Iter + " ";
                    std::vector<std::string> siblin_lines;

                    // brake by spaces
                    while (SourceString.find(" ") != std::string::npos)
                    {
                        size_t BrakeIndex = SourceString.find(" ");

                        if (BrakeIndex > 0)
                        {
                            std::string Value = SourceString.substr(0, BrakeIndex);

                            std::string::size_type n = 0;
                            while ((n = Value.find(":", n)) != std::wstring::npos) {
                                Value.erase(n, 1);
                            }

                            n = 0;
                            while ((n = Value.find(";", n)) != std::wstring::npos) {
                                Value.erase(n, 1);
                            }

                            n = 0;
                            while ((n = Value.find("\t", n)) != std::wstring::npos) {
                                Value.erase(n, 1);
                            }

                            n = 0;
                            while ((n = Value.find("[", n)) != std::wstring::npos) {
                                Value.erase(n);
                            }

                            if (Value != "") {
                                siblin_lines.push_back(Value);
                            }
                        }
                        SourceString = SourceString.substr(BrakeIndex + 1);
                    }

                    // switch tokens
                    if (siblin_lines.size() >= 2)
                    {
                        if (siblin_lines[0] == "bool") {
                            Params_bool.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float4x4") {
                            Params_float4x4.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float4x3") {
                            Params_float4x3.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float3x3") {
                            Params_float3x3.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float4") {
                            Params_float4.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float3") {
                            Params_float3.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "float") {
                            Params_float.push_back(siblin_lines[1]);
                        }
                        else if (siblin_lines[0] == "texture") {
                            Params_texture.push_back(siblin_lines[1]);
                        }
                    }
                }

                bResult = true;
                file.close();
            }
            return bResult;
        }
    }
}
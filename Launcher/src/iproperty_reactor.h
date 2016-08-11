#pragma once

//namespace oes
//{
    namespace nmLauncher
    {
        struct SClassNode
        {
            std::string			name;
            TVecPropertyBase	inheritProperties;
            int					nOverrideByteShift; // overrides byte shift for interface 

            SClassNode(const std::string &_name, int shift = -1)
                : name(_name)
                , nOverrideByteShift(shift)
            {
            }
        };

        //----------------------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------------------
        class IPropertyReactor
        {
        public:
            using TMapClassData = std::vector<SClassNode>;

            virtual ~IPropertyReactor() = 0 {};

            virtual void Build() = 0;
            virtual void Clear() = 0;

            virtual void FetchGroups(std::vector<std::string> &groups) = 0;
            virtual void FetchProperties(const std::string &group, TMapClassData &out) = 0;
        };

    }
//}
#pragma once

//namespace oes
//{
    namespace nmLauncher
    {
        class IPropertyReactor
        {
        public:
            virtual ~IPropertyReactor() = 0 {};

            virtual void Build() = 0;
            virtual void Clear() = 0;
        };

    }
//}
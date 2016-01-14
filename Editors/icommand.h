#pragma once

namespace editors
{
    class ICommand
    {
    public:
        virtual ~ICommand() {} = delete;

        virtual Execute() = 0;
    protected:

    private:
    };
}
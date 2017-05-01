#pragma once

#include <functional>

namespace oes
{
	namespace rflex
	{
		class IGenerator
		{
			public:
				virtual ~IGenerator() = 0 {};

                virtual void instance(const std::function<void()> &f) = 0;
		};
	}
}
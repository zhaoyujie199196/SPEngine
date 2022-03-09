#pragma once

#include <functional>

namespace OpenGLDemos {
	namespace Util {
		class ScopeGuard {
		public:
			ScopeGuard(const std::function<void()> &f)
			{
				m_f = std::move(f);
			}

			~ScopeGuard()
			{
				m_f();
			}

		protected:
			std::function<void()> m_f;
		};
	}
}

#define OD_SCOPEGUARD(f) OpenGLDemos::Util::ScopeGuard guard(f);
#define OD_NAMED_SCOPEGUARD(guardName, f) OpenGLDemos::Util::ScopeGuard guardName(f);
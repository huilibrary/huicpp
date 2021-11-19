

#include "pool.h"


namespace HUICPP {

namespace hc_internal {

Pool::Pool () noexcept
    : m_nChildCount (0) {  }


Pool::Pool (HUN nChildNum) noexcept
    : m_nChildCount (nChildNum) {  }


Pool::~Pool() { }


}
}
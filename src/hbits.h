/**********************************************************
 * Created: Joseph Mar-29-2021
 * Description: 
 *      Wrapper for fixed-length sequence.
 * 
 **********************************************************/

#include "huicpp.h"
#include "utils/base_utils.h"

namespace HUICPP {

    template <HUN _BITS>
    class basic_bits {
    protected:
        using byte_t = HUCH;
        static constexpr HUN BITS = _BITS;
        static constexpr HUN BYTE_WIDTH = sizeof(byte_t);
        static constexpr HUN WIDTH = utils::uper_divied(BITS, BYTE_WIDTH);

    public:
        /* construct 0 value by default */
        constexpr basic_bits(): m_data{0} { }

        /* construct for constant between 0 and 255 */
        constexpr explicit basic_bits(byte_t v): m_data{v} { }

    public:

    private:
        byte_t m_data[WIDTH];
    };


#include "inl/hbits_inl.h"

}


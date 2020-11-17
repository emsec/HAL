#include "bitstream_parser/hdl_parser_bitstream.h"

#include "hal_core/utilities/log.h"
#include "hal_core/utilities/utils.h"

#include <algorithm>
#include <iomanip>

namespace hal
{
    HDLParserBitstream::HDLParserBitstream(const Configuration& config)
    {
        m_config = config;
    }

    bool HDLParserBitstream::parse(const std::filesystem::path& file_name)
    {
        UNUSED(file_name);
        return true;
    }

}    // namespace hal

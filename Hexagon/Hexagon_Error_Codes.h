#ifndef HEXAGON_ERROR_CODES_H
#define HEXAGON_ERROR_CODES_H

namespace Hexagon_Error_Codes {
    enum Error_Code {
        OK,
        READ_ERROR,
        READ_PATCH_ERROR,
        READ_MODIFIED_ERROR,
        WRITE_ERROR,
        PARSE_ERROR,
        FINDINGS_DETECTED,
        SIZE_DIFFERENCE,
        OFFSET_OUT_OF_RANGE,
        BAD_CHECKSUM
    };
}

#endif // HEXAGON_ERROR_CODES_H

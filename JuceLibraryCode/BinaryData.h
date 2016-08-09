/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_649241_INCLUDED
#define BINARYDATA_H_649241_INCLUDED

namespace BinaryData
{
    extern const char*   ICONB_png;
    const int            ICONB_pngSize = 41225;

    extern const char*   next_svg;
    const int            next_svgSize = 575;

    extern const char*   pause_svg;
    const int            pause_svgSize = 571;

    extern const char*   play_svg;
    const int            play_svgSize = 526;

    extern const char*   previous_svg;
    const int            previous_svgSize = 599;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 5;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif

/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== next.svg ==================
static const unsigned char temp_binary_data_0[] =
"<?xml version=\"1.0\" ?><svg height=\"12px\" version=\"1.1\" viewBox=\"0 0 12 12\" width=\"12px\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:sketch=\"http://www.bohemiancoding.com/sketch/ns\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"><title/><desc"
"/><defs/><g fill=\"none\" fill-rule=\"evenodd\" id=\"Page-1\" stroke=\"none\" stroke-width=\"1\"><g fill=\"#000000\" id=\"Icons-AV\" transform=\"translate(-88.000000, -131.000000)\"><g id=\"skip-next\" transform=\"translate(88.000000, 131.000000)\"><"
"path d=\"M0,12 L8.5,6 L0,0 L0,12 L0,12 Z M10,0 L10,12 L12,12 L12,0 L10,0 L10,0 Z\" id=\"Shape\"/></g></g></g></svg>";

const char* next_svg = (const char*) temp_binary_data_0;

//================== pause.svg ==================
static const unsigned char temp_binary_data_1[] =
"<?xml version=\"1.0\" ?><svg height=\"14px\" version=\"1.1\" viewBox=\"0 0 12 14\" width=\"12px\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:sketch=\"http://www.bohemiancoding.com/sketch/ns\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"><title/><desc"
"/><defs/><g fill=\"none\" fill-rule=\"evenodd\" id=\"Page-1\" stroke=\"none\" stroke-width=\"1\"><g fill=\"#000000\" id=\"Icons-AV\" transform=\"translate(-214.000000, -46.000000)\"><g id=\"pause\" transform=\"translate(214.000000, 46.000000)\"><path"
" d=\"M0,14 L4,14 L4,0 L0,0 L0,14 L0,14 Z M8,0 L8,14 L12,14 L12,0 L8,0 L8,0 Z\" id=\"Shape\"/></g></g></g></svg>";

const char* pause_svg = (const char*) temp_binary_data_1;

//================== play.svg ==================
static const unsigned char temp_binary_data_2[] =
"<?xml version=\"1.0\" ?><svg height=\"14px\" version=\"1.1\" viewBox=\"0 0 13 14\" width=\"13px\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:sketch=\"http://www.bohemiancoding.com/sketch/ns\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"><title/><desc"
"/><defs/><g fill=\"none\" fill-rule=\"evenodd\" id=\"Page-1\" stroke=\"none\" stroke-width=\"1\"><g fill=\"#000000\" id=\"Icons-AV\" transform=\"translate(-88.000000, -88.000000)\"><g id=\"play-arrow\" transform=\"translate(88.500000, 88.000000)\"><p"
"ath d=\"M0,0 L0,14 L11,7 L0,0 Z\" id=\"Shape\"/></g></g></g></svg>";

const char* play_svg = (const char*) temp_binary_data_2;

//================== previous.svg ==================
static const unsigned char temp_binary_data_3[] =
"<?xml version=\"1.0\" ?><svg height=\"12px\" version=\"1.1\" viewBox=\"0 0 14 12\" width=\"14px\" xmlns=\"http://www.w3.org/2000/svg\" xmlns:sketch=\"http://www.bohemiancoding.com/sketch/ns\" xmlns:xlink=\"http://www.w3.org/1999/xlink\"><title/><desc"
"/><defs/><g fill=\"none\" fill-rule=\"evenodd\" id=\"Page-1\" stroke=\"none\" stroke-width=\"1\"><g fill=\"#000000\" id=\"Icons-AV\" transform=\"translate(-129.000000, -131.000000)\"><g id=\"skip-previous\" transform=\"translate(129.500000, 131.00000"
"0)\"><rect height=\"12\" id=\"Rectangle-path\" width=\"2\" x=\"0\" y=\"0\"/><path d=\"M3.5,6 L12,12 L12,0 L3.5,6 Z\" id=\"Shape\"/></g></g></g></svg>";

const char* previous_svg = (const char*) temp_binary_data_3;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x54ebc718:  numBytes = 575; return next_svg;
        case 0x3175289b:  numBytes = 571; return pause_svg;
        case 0x7000bed9:  numBytes = 526; return play_svg;
        case 0xbd6c551c:  numBytes = 599; return previous_svg;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const char* namedResourceList[] =
{
    "next_svg",
    "pause_svg",
    "play_svg",
    "previous_svg"
};

}

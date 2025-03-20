/*! \file
    \brief SVG-хелперы
 */

#pragma once

//----------------------------------------------------------------------------
#include <cstdint>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

// #include "marty_svg/marty_svg.h"
// marty::svg::
namespace marty {
namespace svg {

//----------------------------------------------------------------------------
template<typename StreamType>
void writeSvg( StreamType &oss
             , int viewSizeX, int viewSizeY
             , const std::string &style
             , const std::string &text
             )
{
    oss << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"100%\" viewBox=\"0 0 " << viewSizeX << " " << viewSizeY << "\" style=\"max-width: 1026px;\">\n";
    oss << style << "\n";
    oss << text << "\n";
    oss << "</svg>\n";
}

//----------------------------------------------------------------------------
template<typename StreamType>
void drawRect( StreamType &oss
             , int  posX , int posY
             , int  sizeX, int sizeY
             , const std::string &itemClass
             , bool roundLeft
             , bool roundRight
             , int  r
             )
{
    // https://stackoverflow.com/questions/34923888/rounded-corner-only-on-one-side-of-svg-rect
    // https://medium.com/@dennismphil/one-side-rounded-rectangle-using-svg-fb31cf318d90

    // https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/stroke-linejoin

    // https://developer.mozilla.org/en-US/docs/Web/CSS/stroke-linejoin

    if (roundLeft && roundRight) // Both rounds
    {
        oss << "<rect x=\"" << posX << "\" y=\"" << posY << "\" width=\"" << sizeX << "\" height=\"" << sizeY << "\" rx=\"" << r << "\" ry=\"" << r << "\" class=\"" << itemClass << "\" />\n";
    }
    else if (!roundLeft && !roundRight) // No rounds at all
    {
        oss << "<rect x=\"" << posX << "\" y=\"" << posY << "\" width=\"" 
            << sizeX << "\" height=\"" << sizeY << "\" class=\"" << itemClass << "\" />\n";
    }
    else if (roundLeft)
    {
        oss << "<path class=\"" << itemClass 
            << "\" d=\"M" << posX+sizeX << "," << posY // +sizeY 
            << " v" << (sizeY)
            << " h-" << (sizeX-r) 
            << " q-"<<  r   << ",0" << " "  << -r << ",-" << r 
            << " v-" << (sizeY-2*r)
            << " q" << "0,-" << r   << " "  <<  r << ",-" << r // 0,-5 5,-5
            //<< " h" << (sizeX-r) 
            << " z" << "\"" << " />\n";
    }
    else // roundRight
    {
        oss << "<path class=\"" << itemClass 
            << "\" d=\"M" << posX << "," << posY 
            << " h" << (sizeX-r) 
            << " q" << r << ",0 " << r << "," << r 
            << " v" << (sizeY-2*r)
            << " q" << "0," << r << " -" << r << "," << r 
            << " h-" << (sizeX-r) 
            << " z" << "\"" << " />\n";
    }

}

//----------------------------------------------------------------------------
template<typename StreamType>
void drawLine( StreamType &oss
             , int  startX, int startY
             , int  endX  , int endY
             , const std::string &lineClass
             )
{
    oss << "<line x1=\"" << startX << "\" y1=\"" << startY << "\" x2=\"" << endX << "\" y2=\"" << endY << "\" class=\"" << lineClass << "\"/>\n";
}

//----------------------------------------------------------------------------
template<typename StreamType>
void drawText( StreamType &oss
             , int  posX, int posY
             , const std::string &text
             , const std::string &textClass
             , const std::string &baseLine  = "auto" // auto|middle|hanging - https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/dominant-baseline
             , const std::string &hAlign    = "start" // start|middle|end   - https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/text-anchor
             )
{
    oss << "<text x=\"" << posX << "\" y=\"" << posY << "\" class=\"" << textClass << "\" dominant-baseline=\"" << baseLine << "\" text-anchor=\"" << hAlign << "\">" << text << "</text>\n";
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace svg
} // namespace marty
// marty::svg::
// #include "marty_svg/marty_svg.h"


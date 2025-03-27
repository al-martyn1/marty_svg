/*! \file
    \brief SVG-хелперы
 */

#pragma once

//----------------------------------------------------------------------------
#include <cstdint>
#include <iterator>

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

// #include "marty_svg/marty_svg.h"
// marty::svg::
namespace marty {
namespace svg {

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
template<typename CharType, typename OutputIterator> inline
OutputIterator appendToOutputEscapeHelper(OutputIterator outIt, const char* ccStr)
{
    for(; *ccStr; ++ccStr)
        *outIt++ = (CharType)*ccStr;
    return outIt;
}

//----------------------------------------------------------------------------
template<typename InputIterator, typename OutputIterator> inline
OutputIterator escapeText(OutputIterator outIt, InputIterator b, InputIterator e)
{
    using CharType = typename InputIterator::value_type;

    for(; b!=e; ++b)
    {
        auto ch = *b;

        switch(ch)
        {
            // https://en.wikipedia.org/w/index.php?title=List_of_XML_and_HTML_character_entity_references&mobile-app=true&theme=dark
            case (CharType)'&' : outIt = appendToOutputEscapeHelper<CharType>(outIt, "&amp;" ); break;
            case (CharType)'<' : outIt = appendToOutputEscapeHelper<CharType>(outIt, "&lt;"  ); break;
            case (CharType)'>' : outIt = appendToOutputEscapeHelper<CharType>(outIt, "&gt;"  ); break;
            case (CharType)'\'': outIt = appendToOutputEscapeHelper<CharType>(outIt, "&apos;"); break;
            case (CharType)'\"': outIt = appendToOutputEscapeHelper<CharType>(outIt, "&quot;"); break;
            default:
                                *outIt++ = ch;
        }
    }

    return outIt;
}

//----------------------------------------------------------------------------
template<typename StringType> inline
StringType escapeText(const StringType &str)
{
    StringType res; res.reserve(str.size());
    escapeText(std::back_inserter(res), str.begin(), str.end());
    return res;
}


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
void pathStart(StreamType &oss, int posX, int posY, const std::string &pathClass=std::string(), bool bAbs=false )
{
    oss << "<path ";
    if (!pathClass.empty())
    {
        oss << "class=\"" << pathClass << "\" ";
    }
    oss << "d=\"" << (bAbs?'M':'m') << " " << posX << " " << posY;
}

//----------------------------------------------------------------------------
template<typename StreamType>
void pathLineTo(StreamType &oss, int posX, int posY, bool bAbs=false)
{
    oss << " " << (bAbs?'L':'l') << " " << posX << " " << posY;
}

//----------------------------------------------------------------------------
template<typename StreamType>
void pathHorzLineTo(StreamType &oss, int posX, bool bAbs=false)
{
    oss << " " << (bAbs?'H':'h') << " " << posX ;
}

//----------------------------------------------------------------------------
template<typename StreamType>
void pathVertLineTo(StreamType &oss, int posY, bool bAbs=false)
{
    oss << " " << (bAbs?'V':'v') << " " << posY ;
}

//----------------------------------------------------------------------------
template<typename StreamType>
void pathQuadraticBezier(StreamType &oss, int cpX, int cpY, int endX, int endY, bool bAbs=false)
{
    oss << " " << (bAbs?'Q':'q') << " " << cpX << " " << cpY << " " << endX << " " << endY;
}

//----------------------------------------------------------------------------
template<typename StreamType>
void pathEnd(StreamType &oss, bool closePath=true)
{
    oss << (closePath ? " z" : "") << "\" />\n";
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
        pathStart(oss, posX+sizeX, posY, itemClass, true /* bAbs */);
        pathVertLineTo(oss, sizeY);
        pathHorzLineTo(oss, -(sizeX-r));
        pathQuadraticBezier(oss, -r, 0, -r, -r);
        pathVertLineTo(oss, -(sizeY-2*r));
        pathQuadraticBezier(oss, 0, -r, r, -r);
        pathHorzLineTo(oss, (sizeX-r));
        pathEnd(oss, true /* closePath */ );

        #if 0
        oss << "<path class=\"" << itemClass 
            << "\" d=\"M" << posX+sizeX << "," << posY // +sizeY 
            << " v" << (sizeY)
            << " h-" << (sizeX-r) 
            << " q-"<<  r   << ",0" << " "  << -r << ",-" << r 
            << " v-" << (sizeY-2*r)
            << " q" << "0,-" << r   << " "  <<  r << ",-" << r // 0,-5 5,-5
            //<< " h" << (sizeX-r) 
            << " z" << "\"" << " />\n";
        #endif
    }
    else // roundRight
    {
        pathStart(oss, posX, posY, itemClass, true /* bAbs */);
        // pathVertLineTo(oss, sizeY);
        pathHorzLineTo(oss, (sizeX-r));
        pathQuadraticBezier(oss, r, 0, r, r);
        pathVertLineTo(oss, (sizeY-2*r));
        pathQuadraticBezier(oss, 0, r, -r, r);
        pathHorzLineTo(oss, -(sizeX-r));
        pathEnd(oss, true /* closePath */ );

        #if 0
        oss << "<path class=\"" << itemClass 
            << "\" d=\"M" << posX << "," << posY 
            << " h" << (sizeX-r) 
            << " q" << r << ",0 " << r << "," << r 
            << " v" << (sizeY-2*r)
            << " q" << "0," << r << " -" << r << "," << r 
            << " h-" << (sizeX-r) 
            << " z" << "\"" << " />\n";
        #endif
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
    oss << "<text x=\"" << posX << "\" y=\"" << posY << "\" class=\"" << textClass << "\" dominant-baseline=\"" << baseLine << "\" text-anchor=\"" << hAlign << "\">" << escapeText(text) << "</text>\n";
}

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

} // namespace svg
} // namespace marty
// marty::svg::
// #include "marty_svg/marty_svg.h"


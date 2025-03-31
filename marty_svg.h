/*! \file
    \brief SVG-хелперы
 */

#pragma once

//----------------------------------------------------------------------------
#include "enums.h"
//
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
void pathStart( StreamType &oss, int posX, int posY
              , int strokeWidth, const std::string &strokeColor
              , const std::string &fillColor=std::string() /* no fill if empty */
              , std::string linejoin=std::string() /* no fill if empty */
              , bool bAbs=false )
{

    oss << "<path ";
    oss << "stroke=\"" << escapeText(strokeColor) << "\" ";
    oss << "stroke-width=\"" << strokeWidth << "\" ";
    if (linejoin.empty())
        linejoin = "miter";
    oss << "stroke-linejoin=\"" << linejoin << "\" ";

    if (!fillColor.empty())
    {
        oss << "fill=\"" << escapeText(fillColor) << "\" ";
    }
    else
    {
        oss << "fill-opacity=\"0\" "; // Непрозрачность - нулевая
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
void drawRectEx( StreamType &oss
               , int  posX , int posY
               , int  sizeX, int sizeY
               , int r
               , int strokeWidth
               , const std::string &strokeColor
               , const std::string &fillColor = std::string() // no fill if empty
               , RoundRectFlags flags=RoundRectFlags::round /* для отладки */  // RoundRectFlags::none
               )
{

    // stroke="blue"
    // fill="purple"
    // fill-opacity="0.5"
    // stroke-opacity="0.8"
    // stroke-width="15" />

    //void pathStart(StreamType &oss, int posX, int posY, const std::string &pathClass=std::string(), bool bAbs=false )

    int cntTopR     = 0;
    int cntBottomR  = 0;
    int cntLeftR    = 0;
    int cntRightR   = 0;

    // int cntHorzR    = 0;
    // int cntVertR    = 0;

    if ((flags&RoundRectFlags::roundLeftTop)!=0)
    {
        ++cntLeftR;
        ++cntTopR ;
        // ++cntHorzR;
        // ++cntVertR;
    }

    if ((flags&RoundRectFlags::roundLeftBottom)!=0)
    {
        ++cntLeftR;
        ++cntBottomR;
    }

    if ((flags&RoundRectFlags::roundRightTop)!=0)
    {
        ++cntRightR;
        ++cntTopR;
    }

    if ((flags&RoundRectFlags::roundRightBottom)!=0)
    {
        ++cntRightR;
        ++cntBottomR;
    }

    if ((flags&RoundRectFlags::roundLeft)==RoundRectFlags::roundLeft)
    {
        // Только слева
        // Нас интересует только высота
        r = std::min(r, sizeY);
    }
    else if ((flags&RoundRectFlags::roundRight)==RoundRectFlags::roundRight)
    {
        // Только справа
        // Нас интересует только высота
        r = std::min(r, sizeY);
    }
    else if ((flags&RoundRectFlags::roundTop)==RoundRectFlags::roundTop)
    {
        // Только сверху
        // Нас интересует только ширина
        r = std::min(r, sizeX);
    }
    else if ((flags&RoundRectFlags::roundBottom)==RoundRectFlags::roundBottom)
    {
        // Только снизу
        // Нас интересует только ширина
        r = std::min(r, sizeX);
    }
    else
    {
        int maxCntR = std::max({cntTopR, cntBottomR, cntLeftR, cntRightR });
        if (maxCntR)
        {
            r = std::min({r, sizeX/maxCntR, sizeY/maxCntR});
        }
    }
    // int cntHorzR    = 0;
    // int cntVertR    = 0;

    // roundLeft          = roundLeftTop    | roundLeftBottom /*!< Left side angles are round */,
    // roundRight         = roundRightTop   | roundRightBottom /*!< Right side angles are round */,
    // roundTop           = roundLeftTop    | roundRightTop /*!< Top side angles are round */,
    // roundBottom        = roundLeftBottom | roundRightBottom /*!< Bottom side angles are round */,

    int topSizeX    = sizeX;
    int bottomSizeX = sizeX;
    int leftSizeY   = sizeY;
    int rightSizeY  = sizeY;

    if ((flags&RoundRectFlags::roundLeftTop)!=0)
    {
        leftSizeY   -= r;
        topSizeX    -= r;
    }

    if ((flags&RoundRectFlags::roundLeftBottom)!=0)
    {
        leftSizeY   -= r;
        bottomSizeX -= r;
    }

    if ((flags&RoundRectFlags::roundRightTop)!=0)
    {
        rightSizeY  -= r;
        topSizeX    -= r;
    }

    if ((flags&RoundRectFlags::roundRightBottom)!=0)
    {
        rightSizeY  -= r;
        bottomSizeX -= r;
    }


    // У нас есть координата верхнего левого угла прямоугольника
    // Начинаем с него
    if ((flags&RoundRectFlags::roundLeftTop)!=0)
    {
        pathStart(oss, posX, posY+r, strokeWidth, strokeColor, fillColor, std::string(), true /* bAbs */ );
        pathQuadraticBezier(oss, 0, -r, r, -r);
    }
    else
    {
        pathStart(oss, posX, posY, strokeWidth, strokeColor, fillColor, std::string(), true /* bAbs */ );
    }

    pathHorzLineTo(oss, topSizeX);

    if ((flags&RoundRectFlags::roundRightTop)!=0)
    {
        pathQuadraticBezier(oss, r, 0, r, r);
    }

    pathVertLineTo(oss, rightSizeY);

    if ((flags&RoundRectFlags::roundRightBottom)!=0)
    {
        pathQuadraticBezier(oss, 0, r, -r, r);
    }

    pathHorzLineTo(oss, -bottomSizeX);

    if ((flags&RoundRectFlags::roundLeftBottom)!=0)
    {
        pathQuadraticBezier(oss, -r, 0, -r, -r);
    }

    pathVertLineTo(oss, -leftSizeY);

    pathEnd(oss, true /* closePath */ );

}


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
             , int startX, int startY
             , int endX  , int endY
             , const std::string &lineClass
             )
{
    oss << "<line x1=\"" << startX << "\" y1=\"" << startY << "\" x2=\"" << endX << "\" y2=\"" << endY << "\" class=\"" << lineClass << "\"/>\n";
}

//----------------------------------------------------------------------------
template<typename StreamType>
void drawLine( StreamType &oss
             , int startX, int startY
             , int endX  , int endY
             , int strokeWidth
             , const std::string &strokeColor
              , std::string linejoin=std::string() /* no fill if empty */
             )
{
    oss << "<line x1=\"" << startX << "\" y1=\"" << startY << "\" x2=\"" << endX << "\" y2=\"" << endY << "\" ";
    oss << "stroke=\"" << escapeText(strokeColor) << "\" ";
    oss << "stroke-width=\"" << strokeWidth << "\" ";
    if (linejoin.empty())
        linejoin = "miter";
    oss << "stroke-linejoin=\"" << linejoin << "\" ";
    
    oss << " />\n";
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


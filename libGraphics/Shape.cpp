//
//  Shape.cpp
//  Shape
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Shape.h"

namespace grp
{
    Contour::Contour() : _drawLimit(LIMIT_NONE), _dispMode(0), _numVertex(0), _offVertex(0)
    {

    }

    Facet::Facet() : _isSide(false), _numVertex(0), _offVertex(0), _numExt(0), _offIndex(0)
        , _visible(true), _texEnv(TEX_REPLACE), _texCombine(COMBINE_MODULATE)
    {
        _idTextures[0] = 0;
        _idTextures[1] = 0;
    }

    Facet::~Facet()
    {
        clearContour();
    }

    void Facet::clearContour()
    {
        for (auto contour : _contours)
            delete contour;
        _contours.clear();
    }

    Shape::Shape()
        : _numInput(0), _numBasic(0), _idxHole(0), _numVertex(0), _offVertex(0)
        , _dispMode(DISP_FILL), _dispLine(DISP_LINES), _dispTri(DISP_TRIANGLES), _cull(CULL_BACK), _wind(WIND_ANTI_CLOCK)
        , _blend(false), _depth(true), _sfactor(BLEND_SRC_ALPHA), _dfactor(BLEND_ONE_MINUS_SRC_ALPHA)
    {

    }

    Shape::~Shape()
    {
        clearFacet();
    }

    void Shape::clearFacet()
    {
        for (auto facet : _facets)
            delete facet;
        _facets.clear();
    }
}

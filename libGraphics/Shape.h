#pragma once
//
//  Shape.h
//  Shape
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Enum.h"
#include "Matrix.h"
#include "Vector.h"
#include "Color.h"
#include <vector>

namespace grp
{
    class Shape;

    class Contour
    {
    public:
        Contour();

        DrawLimit _drawLimit;
        unsigned  _dispMode;
        unsigned  _numVertex;
        unsigned  _offVertex;
    };

    class Facet
    {
    public:
        Facet();
        ~Facet();

        void clearContour();

        bool _isSide;
        unsigned _numVertex;
        unsigned _offVertex;

        unsigned _numExt;
        unsigned _offExt;

        unsigned _numIndex;
        unsigned _offIndex;

        Color _color;

        bool _visible;
        TexEnv _texEnv;
        Combine _texCombine;

        unsigned _idTextures[2];
        std::vector<Contour*> _contours;
    };

    class Shape
    {
    public:
         Shape();
        ~Shape();

        void clearFacet();

        unsigned _numInput;
        unsigned _numBasic;
        unsigned _idxHole;

        unsigned _numVertex;
        unsigned _offVertex;

        DispMode _dispMode;
        DispLine _dispLine;
        DispTri _dispTri;

        Cull _cull;
        Wind _wind;

        bool _blend;
        bool _depth;

        Blend _sfactor;
        Blend _dfactor;

        Vector3 _position;
        Matrix4 _matrix;

        std::vector<Facet*> _facets;
    };
}

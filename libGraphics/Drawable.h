#pragma once
//
//  Drawable.h
//  Drawable
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Macro.h"
#include "Enum.h"
#include "Shape.h"
#include "Vector.h"

namespace grp
{
    class Drawable
    {
    public:
        Drawable(unsigned numShape, unsigned numVertex, Geometry type = GEO_REGULAR);
        Drawable(unsigned numShape, const unsigned* arrNumVertex, Geometry type = GEO_REGULAR);
        virtual ~Drawable();

        void draw(unsigned idxShape = SHAPE_ALL);

        void setDispMode(DispMode mode, unsigned idxShape = SHAPE_ALL);

        void setDispLine(DispLine mode, unsigned idxShape = SHAPE_ALL);

        void setDispTri(DispTri mode, unsigned idxShape = SHAPE_ALL);

        void setCull(Cull mode, unsigned idxShape = SHAPE_ALL);

        void setWind(Wind mode, unsigned idxShape = SHAPE_ALL);

        void setBlend(bool enable, unsigned idxShape = SHAPE_ALL);

        void setDepth(bool enable, unsigned idxShape = SHAPE_ALL);

        void setBlendOP(Blend sfactor, Blend dfactor, unsigned idxShape = SHAPE_ALL);

        void setPosition(float x, float y, float z, unsigned idxShape = SHAPE_ALL);
        void setPosition(const Vector3& position, unsigned idxShape = SHAPE_ALL);

        void setMatrix(const Matrix4& matrix, unsigned idxShape = SHAPE_ALL);

//        void SetAnimate		(const Animate* animate, GLuint idxShape = SHAPE_ALL);

        void setColor(float r, float g, float b, float a, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);
        void setColor(const Color& color, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);

        void setVisible(bool visible, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);

        void setTexEnv(TexEnv texEnv, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);

        void setTexCombine(Combine combine, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);

        void setTexture(unsigned id, TexUnit unit = TEX_UNIT_0, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);

//      void setTexture(const Texture* texture, TexUnit unit = TEX_UNIT_0, GLuint idxShape = SHAPE_ALL, GLuint idxFacet = FACET_ALL);

        void setVertices(const Vector3* vertices, unsigned idxShape = SHAPE_ALL);

        void setCoordArray(const Vector2* coords, TexUnit unit = TEX_UNIT_0, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);

        void setColorArray(const Color* colors, unsigned idxShape = SHAPE_ALL, unsigned idxFacet = FACET_ALL);

        void setColors(const Color* colors, bool isFacet = false);

        void setIndexArray(const unsigned short* indice, const unsigned* arrNumIndex);

    private:

        void preRender();
        void preRender(unsigned idxShape);
        void render(unsigned idxShape);
        void postRender(unsigned idxShape);
        void postRender();

        void getArrayRange(unsigned idxArray, unsigned numArray, unsigned& begin, unsigned& end);

        void genBuffer(unsigned& id, size_t size, BufferType target = ARRAY_BUFFER);
        void setBuffer(unsigned& id, const void* data, size_t offset, size_t size, BufferType target = ARRAY_BUFFER);

        void setRegular(const Vector3* vertices, Shape* shape);
        void setSolid  (const Vector3* vertices, Shape* shape);
        void setPillar (const Vector3* vertices, Shape* shape);
        void setWall   (const Vector3* vertices, Shape* shape);
        void setBSolid (const Vector3* vertices, Shape* shape);
        void setBPillar(const Vector3* vertices, Shape* shape);
        void setBWall  (const Vector3* vertices, Shape* shape);

    private:
        Geometry _geotype;
        Vector3 _position;
        Matrix4	_matrix;

        unsigned _numVertex;
        unsigned _extVertex;

        unsigned _idIndex;
        unsigned _idVertex;
        unsigned _idColors;
        unsigned _idCoords[2];

        bool _hasFace[3];

        std::vector<Shape*> _shapes;
    };
}

//
//  Drawable.cpp
//  Drawable
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

#include "Drawable.h"
#include "libKazmath/GL/matrix.h"

#if defined(TARGET_QT)
#include <QOpenGLFunctions>
#endif

namespace grp
{
    struct DrawableCache
    {
        bool     depth;
        bool     blend;
        Blend    sfactor;
        Blend    dfactor;
        Cull     cull;
        Wind     wind;
        Color    color;
        TexEnv   texEnv;
        Combine  combine;
        unsigned idTex[2];

        DrawableCache()
        {
            clear();
        }

        void clear() {
            depth = true;
            blend = false;
            sfactor = BLEND_SRC_ALPHA;
            dfactor = BLEND_ONE_MINUS_SRC_ALPHA;
            cull = CULL_BACK;
            wind = WIND_ANTI_CLOCK;
            color.clear();
            texEnv = TEX_NULL;
            combine = COMBINE_NULL;
            idTex[0] = idTex[1] = 0;
        }
    };

    static DrawableCache s_cache;

    Drawable::Drawable(unsigned numShape, unsigned numVertex, Geometry type)
    {
        unsigned* arrNumVertex = new unsigned[numShape];
        for (unsigned i = 0; i < numShape; i++)
            arrNumVertex[i] = numVertex;
        *this = Drawable(numShape, arrNumVertex, type);
    }

    Drawable::Drawable(unsigned numShape, const unsigned* arrNumVertex, Geometry type)
        : _geotype(type), _numVertex(0), _extVertex(0), _idIndex(0), _idVertex(0), _idColors(0)
    {
        _idCoords[0] = _idCoords[1] = 0;

        unsigned numSideVertex;
        switch (_geotype)
        {
            case GEO_REGULAR : _hasFace[0] = true;  _hasFace[1] = false; _hasFace[2] = false; numSideVertex = 0; break;
            case GEO_SOLID   : _hasFace[0] = true;	_hasFace[1] = true;  _hasFace[2] = true;  numSideVertex = 4; break;
            case GEO_PILLAR  : _hasFace[0] = true;	_hasFace[1] = false; _hasFace[2] = true;  numSideVertex = 4; break;
            case GEO_WALL    : _hasFace[0] = false; _hasFace[1] = false; _hasFace[2] = true;  numSideVertex = 4; break;
            case GEO_BSOLID  : _hasFace[0] = true;	_hasFace[1] = true;  _hasFace[2] = true;  numSideVertex = 4; break;
            case GEO_BPILLAR : _hasFace[0] = true;	_hasFace[1] = false; _hasFace[2] = true;  numSideVertex = 4; break;
            case GEO_BWALL   : _hasFace[0] = false; _hasFace[1] = false; _hasFace[2] = true;  numSideVertex = 4; break;
            case GEO_CONE    : _hasFace[0] = false; _hasFace[1] = true;	 _hasFace[2] = true;  numSideVertex = 3; break;
            case GEO_CWALL   : _hasFace[0] = false; _hasFace[1] = false; _hasFace[2] = true;  numSideVertex = 3; break;
        }

        for (unsigned idxShape = 0; idxShape < numShape; idxShape++)
        {
            Shape* shape = new Shape();
            shape->_numBasic = arrNumVertex[idxShape];

            switch (_geotype)
            {
                case GEO_SOLID  :
                case GEO_PILLAR :
                case GEO_WALL   :
                    shape->_numInput = shape->_numBasic * 2;
                    break;

                case GEO_CONE  :
                case GEO_CWALL :
                    shape->_numInput = shape->_numBasic + 1;
                    break;

                default :
                    shape->_numInput = shape->_numBasic;
            }

            shape->_offVertex = _numVertex;

            for (unsigned idxFace = 0; idxFace < 2; idxFace++)
            {
                if (_hasFace[idxFace])
                {
                    Facet* facet = new Facet();
                    facet->_offVertex = _numVertex;
                    facet->_numVertex = shape->_numBasic;

                    shape->_facets.push_back(facet);
                    _numVertex += facet->_numVertex;
                }
            }

            if (_hasFace[2])
            {
                unsigned numSide = shape->_numBasic;
                for (unsigned idxSide = 0; idxSide < numSide; idxSide++)
                {
                    Facet* facet = new Facet();
                    facet->_isSide = true;
                    facet->_offVertex = _numVertex;
                    facet->_numVertex = numSideVertex;

                    shape->_facets.push_back(facet);
                    _numVertex += facet->_numVertex;
                }
            }

            shape->_numVertex = _numVertex - shape->_offVertex;
            _shapes.push_back(shape);
        }
    }

    Drawable::~Drawable()
    {
        for (auto shape : _shapes)
            delete shape;
        _shapes.clear();

        if (_idIndex)
            glDeleteBuffers(1, &_idIndex);

        if (_idVertex)
            glDeleteBuffers(1, &_idVertex);

        if (_idColors)
            glDeleteBuffers(1, &_idColors);

        if (_idCoords[0])
            glDeleteBuffers(1, &_idCoords[0]);

        if (_idCoords[1])
            glDeleteBuffers(1, & _idCoords[1]);
    }

    void Drawable::draw(unsigned idxShape)
    {
        preRender();
        render(idxShape);
        postRender();
    }

    void Drawable::setDispMode(DispMode mode, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];
            shape->_dispMode = mode;
        }
    }

    void Drawable::setDispLine(DispLine mode, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];
            shape->_dispLine = mode;
        }
    }

    void Drawable::setDispTri(DispTri mode, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];
            shape->_dispTri = mode;
        }
    }

    void Drawable::setCull(Cull mode, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape*  shape = _shapes[idxShape];
            shape->_cull = mode;
        }
    }

    void Drawable::setWind(Wind mode, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape*  shape = _shapes[idxShape];
            shape->_wind = mode;
        }
    }

    void Drawable::setBlend(bool enable, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape*  shape = _shapes[idxShape];
            shape->_blend = enable;
        }
    }

    void Drawable::setDepth(bool enable, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape*  shape = _shapes[idxShape];
            shape->_depth = enable;
        }
    }

    void Drawable::setBlendOP(Blend sfactor, Blend dfactor, unsigned idxShape_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];
            shape->_sfactor = sfactor;
            shape->_dfactor = dfactor;
        }
    }

    void Drawable::setPosition(float x, float y, float z, unsigned idxShape)
    {
        setPosition(Vector3(x, y, z), idxShape);
    }

    void Drawable::setPosition(const Vector3& position, unsigned idxShape)
    {
        if (idxShape == SHAPE_ALL)
        {
            _position = position;
        }
        else
        {
            Shape* shape = _shapes[idxShape];
            shape->_position = position;
        }
    }

    void Drawable::setMatrix(const Matrix4& matrix, unsigned idxShape)
    {
        if (idxShape == SHAPE_ALL)
        {
            _matrix = matrix;
        }
        else
        {
            Shape* shape = _shapes[idxShape];
            shape->_matrix = matrix;
        }
    }

    void Drawable::setColor(float r, float g, float b, float a, unsigned idxShape, unsigned idxFacet)
    {
        setColor(Color(r, g, b, a), idxShape, idxFacet);
    }

    void Drawable::setColor(const Color& color, unsigned idxShape_, unsigned idxFacet_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange (idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];

            unsigned idxFacet, numFacet;
            for (getArrayRange(idxFacet_, shape->_facets.size(), idxFacet, numFacet); idxFacet < numFacet; idxFacet++)
            {
                Facet* facet = shape->_facets[idxFacet];
                facet->_color = color;
            }
        }
    }

    void Drawable::setColors(const Color* colors, bool isFacet)
    {
        unsigned idxColor = 0;
        for (Shape* shape : _shapes)
        {
            for (Facet* facet : shape->_facets)
            {
                facet->_color = colors[idxColor];
                if (isFacet)
                    idxColor += 1;
            }

            if (!isFacet)
                idxColor += 1;
        }
    }

    void Drawable::setVisible(bool visible, unsigned idxShape_, unsigned idxFacet_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];

            unsigned idxFacet, numFacet;
            for (getArrayRange(idxFacet_, shape->_facets.size(), idxFacet, numFacet); idxFacet < numFacet; idxFacet++)
            {
                Facet* facet = shape->_facets[idxFacet];
                facet->_visible = visible;
            }
        }
    }

    void Drawable::setTexEnv(TexEnv texEnv, unsigned idxShape_, unsigned idxFacet_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];

            unsigned idxFacet, numFacet;
            for (getArrayRange(idxFacet_, shape->_facets.size(), idxFacet, numFacet); idxFacet < numFacet; idxFacet++)
            {
                Facet* facet = shape->_facets[idxFacet];
                facet->_texEnv = texEnv;
            }
        }
    }

    void Drawable::setTexCombine(Combine combine, unsigned idxShape_, unsigned idxFacet_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];

            unsigned idxFacet, numFacet;
            for (getArrayRange(idxFacet_, shape->_facets.size(), idxFacet, numFacet); idxFacet < numFacet; idxFacet++)
            {
                Facet* facet = shape->_facets[idxFacet];
                facet->_texCombine = combine;
            }
        }
    }

    void Drawable::setTexture(unsigned id, TexUnit unit, unsigned idxShape_, unsigned idxFacet_)
    {
        unsigned idxShape, numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];

            unsigned idxFacet, numFacet;
            for (getArrayRange(idxFacet_, shape->_facets.size(), idxFacet, numFacet); idxFacet < numFacet; idxFacet++)
            {
                Facet* facet = shape->_facets[idxFacet];
                facet->_idTextures[unit] = id;
            }
        }
    }
#if 0
    void Render::SetTexture(const Texture* texture, TexUnit unit, unsigned idxShape, unsigned idxFacet)
    {
        Texture*		tex = (Texture *) texture;
        unsigned			id;

        tex->GetID(id);
        SetTexture(id, unit, idxShape, idxFacet);
    }
#endif
    void Drawable::setVertices(const Vector3* vertices, unsigned idxShape_)
    {
        if (idxShape_ == SHAPE_ALL)
        {
            if (_geotype == GEO_REGULAR)
            {
                genBuffer(_idVertex, sizeof(Vector3) * (_numVertex + _extVertex));
                setBuffer(_idVertex, vertices, 0, sizeof(Vector3) * _numVertex);
                return;
            }
            else
            {
                genBuffer(_idVertex, sizeof(Vector3) * (_numVertex + _extVertex));
            }
        }
        else if (!_idVertex)
        {
            genBuffer(_idVertex, sizeof(Vector3) * (_numVertex + _extVertex));
        }

        unsigned offVertex = 0;
        unsigned idxShape = 0;
        unsigned numShape = 0;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];
            switch (_geotype)
            {
                case GEO_REGULAR : setRegular((const Vector3*) &vertices[offVertex], shape); break;
                case GEO_SOLID	 : setSolid  ((const Vector3*) &vertices[offVertex], shape); break;
                case GEO_PILLAR	 : setPillar ((const Vector3*) &vertices[offVertex], shape); break;
                case GEO_WALL	 : setWall   ((const Vector3*) &vertices[offVertex], shape); break;
                case GEO_BSOLID	 : setBSolid ((const Vector3*) &vertices[offVertex], shape); break;
                case GEO_BPILLAR : setBPillar((const Vector3*) &vertices[offVertex], shape); break;
                case GEO_BWALL	 : setBWall  ((const Vector3*) &vertices[offVertex], shape); break;

                default:
                    break;
            }
            offVertex += shape->_numInput;
        }
    }

    void Drawable::setCoordArray(const Vector2* coords, TexUnit unit, unsigned idxShape_, unsigned idxFacet_)
    {
        if (idxShape_ == SHAPE_ALL && idxFacet_ == FACET_ALL)
        {
            genBuffer(_idCoords[unit], sizeof(Vector2) * (_numVertex + _extVertex));
            setBuffer(_idCoords[unit], coords, 0, sizeof(Vector2) * _numVertex);
        }
        else
        {
            if (_idCoords[unit] == 0)
            {
                genBuffer(_idCoords[unit], sizeof(Vector2) * (_numVertex + _extVertex));
            }

            unsigned offSrc = 0;
            unsigned idxShape = 0;
            unsigned numShape = 0;
            for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
            {
                Shape* shape = _shapes[idxShape];

                unsigned idxFacet = 0;
                unsigned numFacet = 0;
                for (getArrayRange(idxFacet_, shape->_facets.size(), idxFacet, numFacet); idxFacet < numFacet; idxFacet++)
                {
                    Facet* facet = shape->_facets[idxFacet];
                    setBuffer(_idCoords[unit], &coords[offSrc], sizeof(Vector2) * facet->_offVertex, sizeof(Vector2) * facet->_numVertex);
                    offSrc += facet->_numVertex;
                }
            }
        }
    }

    void Drawable::setColorArray(const Color* colors, unsigned idxShape_, unsigned idxFacet_)
    {
        genBuffer(_idColors, sizeof(Color) * (_numVertex + _extVertex));
        if (idxShape_ == SHAPE_ALL && idxFacet_ == FACET_ALL)
        {
            setBuffer(_idColors, colors, 0, sizeof(Color) * _numVertex);
        }
        else
        {
            unsigned offset = 0;
            unsigned idxShape, numShape;
            for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
            {
                Shape* shape = _shapes[idxShape];

                unsigned idxFacet, numFacet;
                for (getArrayRange(idxFacet_, shape->_facets.size(), idxFacet, numFacet); idxFacet < numFacet; idxFacet++)
                {
                    Facet* facet = shape->_facets[idxFacet];
                    setBuffer(_idColors, &colors[offset], sizeof(Color) * facet->_offVertex, sizeof(Color) * facet->_numVertex);
                    offset += facet->_numVertex;
                }
            }
        }
    }

    void Drawable::setIndexArray(const unsigned short* indice, const unsigned* arrNumIndex)
    {
        unsigned numIndex = 0;

        for (unsigned idxShape = 0; idxShape < _shapes.size(); idxShape++)
        {
            Shape* shape = _shapes[idxShape];

            for (unsigned idxFacet = 0; idxFacet < shape->_facets.size(); idxFacet++)
            {
                Facet* facet = shape->_facets[idxShape];

                facet->_offIndex = numIndex;
                facet->_numIndex = arrNumIndex[idxShape];

                numIndex += arrNumIndex[idxShape];
            }
        }

        genBuffer(_idIndex, sizeof(GLushort) * numIndex, ELEMENT_ARRAY_BUFFER);
        setBuffer(_idIndex, indice, 0, sizeof(GLushort) * numIndex);
    }

    void Drawable::preRender()
    {
        s_cache.clear();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        glDisable(GL_BLEND);
        glBlendFunc(s_cache.sfactor, s_cache.dfactor);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glFrontFace(s_cache.wind);

        kmGLPushMatrix();
        kmGLTranslatef(_position.x, _position.y, _position.z);
        kmGLMultMatrix((const kmMat4*) &_matrix);

        if (_idVertex)
        {            
            glBindBuffer(GL_ARRAY_BUFFER, _idVertex);
            //glEnableClientState (GL_VERTEX_ARRAY);
            //glVertexPointer( 3, m_type_vertex, 0, 0);
        }
        else
        {
            //glDisableClientState(GL_VERTEX_ARRAY);
        }


        if (_idColors)
        {            
            glBindBuffer(GL_ARRAY_BUFFER, _idColors);
            //glEnableClientState(GL_COLOR_ARRAY);
            //glColorPointer(4, m_type_colors, 0, 0);
        }
        else
        {
            //glDisableClientState(GL_COLOR_ARRAY);
        }

        if (_idCoords[TEX_UNIT_0])
        {
            glActiveTexture(GL_TEXTURE0);
            glBindBuffer(GL_ARRAY_BUFFER, _idCoords[TEX_UNIT_0]);
            //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            //glTexCoordPointer( 2, m_type_coords[TEX_UNIT_0], 0, 0);

            if (_idCoords[TEX_UNIT_1])
            {
                glActiveTexture(GL_TEXTURE1);
                glBindBuffer(GL_ARRAY_BUFFER, _idCoords[TEX_UNIT_1]);
                //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                //glTexCoordPointer(2, m_type_coords[TEX_UNIT_1], 0, 0);
            }
            else
            {
                glActiveTexture(GL_TEXTURE1);
                //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
                glActiveTexture(GL_TEXTURE0);
            }
        }
        else
        {
            glActiveTexture(GL_TEXTURE1);
            //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

            glActiveTexture(GL_TEXTURE0);
            //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        }

        if (_idIndex)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _idIndex);
        }
    }

    void Drawable::preRender(unsigned idxShape)
    {
        Shape* shape = _shapes[idxShape];

        if (s_cache.depth != shape->_depth)
        {
            s_cache.depth = shape->_depth;
            if (s_cache.depth)
            {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
        }

        if (s_cache.blend != shape->_blend)
        {
            s_cache.blend = shape->_blend;
            if (s_cache.blend)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
        }

        if (s_cache.blend && (s_cache.sfactor != shape->_sfactor || s_cache.dfactor != shape->_dfactor))
        {
            s_cache.sfactor = shape->_sfactor;
            s_cache.dfactor = shape->_dfactor;
            glBlendFunc(s_cache.sfactor, s_cache.dfactor);
        }

        if (s_cache.cull != shape->_cull)
        {
            s_cache.cull = shape->_cull;
            switch (s_cache.cull)
            {
                case CULL_DISABLE :	glDisable(GL_CULL_FACE); break;
                case CULL_FRONT   :	glEnable (GL_CULL_FACE); glCullFace(GL_FRONT); break;
                case CULL_BACK    :	glEnable (GL_CULL_FACE); glCullFace(GL_BACK);  break;
            }
        }

        if (s_cache.wind != shape->_wind)
        {
            s_cache.wind = shape->_wind;
            glFrontFace(s_cache.wind);
        }

        kmGLPushMatrix();
        kmGLTranslatef(_position.x, _position.y, _position.z);
        kmGLMultMatrix((const kmMat4*) &shape->_matrix);
    }

    void Drawable::render(unsigned idxShape_)
    {
        unsigned idxShape;
        unsigned numShape;
        for (getArrayRange(idxShape_, _shapes.size(), idxShape, numShape); idxShape < numShape; idxShape++)
        {
            Shape* shape = _shapes[idxShape];
            preRender(idxShape);

            unsigned dispShape = 0;
            switch (shape->_dispMode)
            {
                case DISP_FILL    :
                case DISP_TEXTURE :	dispShape = shape->_dispTri;  break;
                case DISP_LINE	  : dispShape = shape->_dispLine; break;
                case DISP_POINT	  : dispShape = GL_POINTS;		  break;
            }

            unsigned numFacet = shape->_facets.size();
            for (unsigned idxFacet = 0; idxFacet < numFacet; idxFacet++)
            {
                Facet* facet = shape->_facets[idxFacet];
                if (!facet->_visible)
                    continue;

                if (s_cache.color != facet->_color)
                {
                    s_cache.color = facet->_color;
//                    glColor4f(s_cache.color.r, s_cache.color.g, s_cache.color.b, s_cache.color.a);
                }

                if (shape->_dispMode == DISP_TEXTURE)
                {
                    if (_idCoords[TEX_UNIT_0] && facet->_idTextures[TEX_UNIT_0])
                    {
                        if (s_cache.idTex[TEX_UNIT_0] != facet->_idTextures[TEX_UNIT_0] || s_cache.texEnv != facet->_texEnv)
                        {
                            s_cache.idTex[TEX_UNIT_0] = facet->_idTextures[TEX_UNIT_0];
                            s_cache.texEnv = facet->_texEnv;

                            glActiveTexture(GL_TEXTURE0);
                            glEnable(GL_TEXTURE_2D);
                            glBindTexture(GL_TEXTURE_2D, facet->_idTextures [TEX_UNIT_0]);
//                          glTexEnv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, (GLfloat) facet->_texEnv);
                        }

                        if (_idCoords[TEX_UNIT_1] && facet->_idTextures[TEX_UNIT_1])
                        {
                            if (s_cache.idTex[TEX_UNIT_1] != facet->_idTextures[TEX_UNIT_1] || s_cache.combine != facet->_texCombine)
                            {
                                s_cache.idTex[TEX_UNIT_1] = facet->_idTextures[TEX_UNIT_1];
                                s_cache.combine = facet->_texCombine;

                                glActiveTexture(GL_TEXTURE1);
                                glEnable(GL_TEXTURE_2D);
                                glBindTexture(GL_TEXTURE_2D, facet->_idTextures [TEX_UNIT_1]);

                                //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
                                //glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB, (GLfloat) facet->_texCombine);

                                //glTexEnvf(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
                                //glTexEnvf(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);

                                //glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
                                //glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
                            }
                        }                        
                    }
                    else                   
                    {
                        s_cache.idTex[TEX_UNIT_0] = 0;
                        s_cache.idTex[TEX_UNIT_1] = 0;

                        glActiveTexture(GL_TEXTURE0);
                        glDisable(GL_TEXTURE_2D);
                    }                    
                }

                unsigned numContour = facet->_contours.size ();
                if (numContour > 0)
                {
                    for (unsigned idxContour = 0; idxContour < numContour; idxContour++)
                    {
                        Contour* contour = facet->_contours[idxContour];
                        unsigned dispContour;
                        switch (contour->_drawLimit)
                        {
                            case LIMIT_TRI :
                                if (shape->_dispMode == DISP_POINT || shape->_dispMode == DISP_LINE)
                                    continue;                                
                                dispContour = contour->_dispMode ? contour->_dispMode : dispShape;
                                break;

                            case LIMIT_LINE :
                                if (shape->_dispMode == DISP_FILL || shape->_dispMode == DISP_TEXTURE)
                                    continue;                                

                            case LIMIT_NONE :
                                dispContour = dispShape;
                        }
                        glDrawArrays(dispContour, contour->_offVertex, contour->_numVertex);
                    }
                }
                else
                {
                    if (_idIndex)
                        glDrawElements(dispShape, facet->_numIndex, GL_UNSIGNED_SHORT, &((GLshort*) 0)[sizeof (GLushort) * facet->_offIndex]);
                    else
                    {
                        if (facet->_isSide)
                        {
                            unsigned dispFacet;
                            switch (shape->_dispMode)
                            {
                                case DISP_FILL    :
                                case DISP_TEXTURE :	dispFacet = GL_TRIANGLE_FAN; break;
                                case DISP_LINE	  : dispFacet = GL_LINE_LOOP; break;
                                case DISP_POINT	  : dispFacet = GL_POINTS; break;
                            }
                            glDrawArrays(dispFacet, facet->_offVertex, facet->_numVertex);
                        }
                        else
                        {
                            glDrawArrays(dispShape, facet->_offVertex, facet->_numVertex);
                        }
                    }
                }                
            }
            postRender(idxShape);
        }        
    }

    void Drawable::postRender(unsigned)
    {
        kmGLPopMatrix();
    }

    void Drawable::postRender()
    {
        glActiveTexture(GL_TEXTURE1);
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);

        //glDisableClientState(GL_COLOR_ARRAY);
        //glDisableClientState(GL_VERTEX_ARRAY);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        kmGLPopMatrix();
    }

    void Drawable::getArrayRange(unsigned idxArray, unsigned numArray, unsigned& begin, unsigned& end)
    {
        if (idxArray == 0xfffffffF)
        {
            begin = 0;
            end = numArray;
        }
        else
        {
            begin = idxArray;
            end = begin + 1;
        }
    }

    void Drawable::setRegular(const Vector3* vertices, Shape* shape)
    {
        setBuffer(_idVertex, vertices, sizeof(Vector3) * shape->_offVertex, sizeof(Vector3) * shape->_numVertex);
    }

    void Drawable::setSolid(const Vector3* vertices, Shape* shape)
    {
        // Front & Rear
        setBuffer(_idVertex, vertices, sizeof(Vector3) * shape->_offVertex, sizeof(Vector3) * shape->_numBasic * 2);

        // Side
        Vector3* verticesSide = new Vector3[shape->_numBasic*4];

        for (unsigned idxVertex1 = 0; idxVertex1 < shape->_numBasic; idxVertex1++)
        {
            unsigned idxVertex2;
            if (shape->_idxHole == HOLE_NULL)
                idxVertex2 = (idxVertex1 + 1) < shape->_numBasic ? idxVertex1 + 1 : 0;
            else
                idxVertex2 = (idxVertex1 + 1) == shape->_idxHole ? 0 : (idxVertex1 + 1) == shape->_numBasic ? shape->_idxHole : idxVertex1 + 1;

            verticesSide[idxVertex1*4 + 0] = vertices[idxVertex1];
            verticesSide[idxVertex1*4 + 1] = vertices[shape->_numInput - idxVertex1 - 1];
            verticesSide[idxVertex1*4 + 2] = vertices[shape->_numInput - idxVertex2 - 1];
            verticesSide[idxVertex1*4 + 3] = vertices[idxVertex2];
        }

        setBuffer(_idVertex, verticesSide, sizeof(Vector3) * (shape->_offVertex + shape->_numBasic * 2), sizeof(Vector3) * shape->_numBasic*4);
        delete [] verticesSide;
    }

    void Drawable::setPillar(const Vector3* vertices, Shape* shape)
    {
        // Front
        setBuffer(_idVertex, vertices, sizeof(Vector3) * shape->_offVertex, sizeof(Vector3) * shape->_numBasic);

        // Side
        Vector3* verticesSide = new Vector3[shape->_numBasic*4];

        for (unsigned idxVertex1 = 0; idxVertex1 < shape->_numBasic; idxVertex1++)
        {
            unsigned idxVertex2;
            if (shape->_idxHole == HOLE_NULL)
                idxVertex2 = (idxVertex1 + 1) < shape->_numBasic ? idxVertex1 + 1 : 0;
            else
                idxVertex2 = (idxVertex1 + 1) == shape->_idxHole ? 0 : (idxVertex1 + 1) == shape->_numBasic ? shape->_idxHole : idxVertex1 + 1;

            verticesSide[idxVertex1*4 + 0] = vertices[idxVertex1];
            verticesSide[idxVertex1*4 + 1] = vertices[shape->_numInput - idxVertex1 - 1];
            verticesSide[idxVertex1*4 + 2] = vertices[shape->_numInput - idxVertex2 - 1];
            verticesSide[idxVertex1*4 + 3] = vertices[idxVertex2];
        }

        setBuffer(_idVertex, verticesSide, sizeof(Vector3) * (shape->_offVertex + shape->_numBasic), sizeof(Vector3) * shape->_numBasic*4);
        delete [] verticesSide;
    }

    void Drawable::setWall(const Vector3* vertices, Shape* shape)
    {
        // Side
        Vector3* verticesSide = new Vector3[shape->_numBasic*4];

        for (unsigned idxVertex1 = 0; idxVertex1 < shape->_numBasic; idxVertex1++)
        {
            unsigned idxVertex2;
            if (shape->_idxHole == HOLE_NULL)
                idxVertex2 = (idxVertex1 + 1) < shape->_numBasic ? idxVertex1 + 1 : 0;
            else
                idxVertex2 = (idxVertex1 + 1) == shape->_idxHole ? 0 : (idxVertex1 + 1) == shape->_numBasic ? shape->_idxHole : idxVertex1 + 1;

            verticesSide[idxVertex1*4 + 0] = vertices[idxVertex1];
            verticesSide[idxVertex1*4 + 1] = vertices[shape->_numInput - idxVertex1 - 1];
            verticesSide[idxVertex1*4 + 2] = vertices[shape->_numInput - idxVertex2 - 1];
            verticesSide[idxVertex1*4 + 3] = vertices[idxVertex2];
        }

        setBuffer(_idVertex, verticesSide, sizeof(Vector3) * (shape->_offVertex), sizeof(Vector3) * shape->_numBasic*4);
        delete [] verticesSide;
    }

    void Drawable::setBSolid(const Vector3* vertices, Shape* shape)
    {
        // Front
        setBuffer(_idVertex, vertices, sizeof(Vector3) * shape->_offVertex, sizeof(Vector3) * shape->_numBasic);

        // Rear
        Vector3* verticesRear = new Vector3[shape->_numBasic];

        for (unsigned idxVertex1 = 0; idxVertex1 < shape->_numBasic; idxVertex1++)
        {
            unsigned idxVertex2;
            if (shape->_idxHole == HOLE_NULL)
                idxVertex2 = shape->_numBasic - idxVertex1 - 1;
            else
                idxVertex2 = idxVertex1 < shape->_idxHole ? shape->_idxHole - idxVertex1 - 1 : shape->_numBasic + shape->_idxHole - idxVertex1 - 1;

            verticesRear[idxVertex1].x = vertices[idxVertex2].x;
            verticesRear[idxVertex1].y = vertices[idxVertex2].y;
            verticesRear[idxVertex1].z = 0;
        }

        setBuffer(_idVertex, verticesRear, sizeof(Vector3) * (shape->_offVertex + shape->_numBasic), sizeof(Vector3) * shape->_numBasic);
        delete [] verticesRear;

        // Side
        Vector3* verticesSide = new Vector3[shape->_numBasic*4];

        for (unsigned idxVertex1 = 0; idxVertex1 < shape->_numBasic; idxVertex1++)
        {
            unsigned idxVertex2;
            if (shape->_idxHole == HOLE_NULL)
                idxVertex2 = (idxVertex1 + 1) < shape->_numBasic ? idxVertex1 + 1 : 0;
            else
                idxVertex2 = (idxVertex1 + 1) == shape->_idxHole ? 0 : (idxVertex1 + 1) == shape->_numBasic ? shape->_idxHole : idxVertex1 + 1;

            verticesSide[idxVertex1*4 + 0] = vertices[idxVertex1];

            verticesSide[idxVertex1*4 + 1].x = vertices[idxVertex1].x;
            verticesSide[idxVertex1*4 + 1].y = vertices[idxVertex1].y;
            verticesSide[idxVertex1*4 + 1].z = 0;

            verticesSide[idxVertex1*4 + 2].x = vertices[idxVertex2].x;
            verticesSide[idxVertex1*4 + 2].y = vertices[idxVertex2].y;
            verticesSide[idxVertex1*4 + 2].z = 0;

            verticesSide[idxVertex1*4 + 3] = vertices[idxVertex2];
        }

        setBuffer(_idVertex, verticesSide, sizeof(Vector3) * (shape->_offVertex + shape->_numBasic * 2), sizeof(Vector3) * shape->_numBasic*4);
        delete [] verticesSide;
    }

    void Drawable::setBPillar(const Vector3* vertices, Shape* shape)
    {
        // Front
        setBuffer(_idVertex, vertices, sizeof(Vector3) * shape->_offVertex, sizeof(Vector3) * shape->_numBasic);

        // Side
        Vector3* verticesSide = new Vector3[shape->_numBasic*4];
        for (unsigned idxVertex1 = 0; idxVertex1 < shape->_numBasic; idxVertex1++)
        {
            unsigned idxVertex2;
            if (shape->_idxHole == HOLE_NULL)
                idxVertex2 = (idxVertex1 + 1) < shape->_numBasic ? idxVertex1 + 1 : 0;
            else
                idxVertex2 = (idxVertex1 + 1) == shape->_idxHole ? 0 : (idxVertex1 + 1) == shape->_numBasic ? shape->_idxHole : idxVertex1 + 1;

            verticesSide[idxVertex1*4 + 0] = vertices[idxVertex1];

            verticesSide[idxVertex1*4 + 1].x = vertices[idxVertex1].x;
            verticesSide[idxVertex1*4 + 1].y = vertices[idxVertex1].y;
            verticesSide[idxVertex1*4 + 1].z = 0;

            verticesSide[idxVertex1*4 + 2].x = vertices[idxVertex2].x;
            verticesSide[idxVertex1*4 + 2].y = vertices[idxVertex2].y;
            verticesSide[idxVertex1*4 + 2].z = 0;

            verticesSide[idxVertex1*4 + 3] = vertices[idxVertex2];
        }

        setBuffer(_idVertex, verticesSide, sizeof(Vector3) * (shape->_offVertex + shape->_numBasic), sizeof(Vector3) * shape->_numBasic*4);
        delete [] verticesSide;
    }

    void Drawable::setBWall(const Vector3* vertices, Shape* shape)
    {
        // Side
        Vector3* verticesSide = new Vector3[shape->_numBasic*4];
        for (unsigned idxVertex1 = 0; idxVertex1 < shape->_numBasic; idxVertex1++)
        {
            unsigned idxVertex2;
            if (shape->_idxHole == HOLE_NULL)
            {
                idxVertex2 = (idxVertex1 + 1) < shape->_numBasic ? idxVertex1 + 1 : 0;
            }
            else
            {
                idxVertex2 = (idxVertex1 + 1) == shape->_idxHole ? 0 : (idxVertex1 + 1) == shape->_numBasic ? shape->_idxHole : idxVertex1 + 1;
            }

            verticesSide[idxVertex1*4 + 0] = vertices[idxVertex1];

            verticesSide[idxVertex1*4 + 1].x = vertices[idxVertex1].x;
            verticesSide[idxVertex1*4 + 1].y = vertices[idxVertex1].y;
            verticesSide[idxVertex1*4 + 1].z = 0;

            verticesSide[idxVertex1*4 + 2].x = vertices[idxVertex2].x;
            verticesSide[idxVertex1*4 + 2].y = vertices[idxVertex2].y;
            verticesSide[idxVertex1*4 + 2].z = 0;

            verticesSide[idxVertex1*4 + 3] = vertices[idxVertex2];
        }

        setBuffer(_idVertex, verticesSide, sizeof(Vector3) * (shape->_offVertex), sizeof(Vector3) * shape->_numBasic*4);
        delete [] verticesSide;
    }

    void Drawable::genBuffer(unsigned& id, size_t size, BufferType target_)
    {
        GLenum target;
        switch (target_)
        {
            case ARRAY_BUFFER: target = GL_ARRAY_BUFFER; break;
            case ELEMENT_ARRAY_BUFFER: target = GL_ELEMENT_ARRAY_BUFFER; break;
        }

        if (id)
            glDeleteBuffers (1, &id);

        glGenBuffers(1, &id);
        glBindBuffer(target, id);
        glBufferData(target, size, 0, GL_STATIC_DRAW);
    }

    void Drawable::setBuffer(unsigned& id, const void* data, size_t offset, size_t size, BufferType target_)
    {
        GLenum target;
        switch (target_)
        {
            case ARRAY_BUFFER: target = GL_ARRAY_BUFFER; break;
            case ELEMENT_ARRAY_BUFFER: target = GL_ELEMENT_ARRAY_BUFFER; break;
        }

        glBindBuffer(target, id);
        glBufferSubData(target, offset, size, data);
    }
}

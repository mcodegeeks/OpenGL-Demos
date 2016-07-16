#pragma once
//
//  Color.h
//  Color
//
//  Created by Younghwan Mun on 2016-07-13.
//  Copyright © 2016 MCodeGeeks. All rights reserved.
//

namespace grp
{
    class Color
    {
    public:
        Color();
        Color(float r, float g, float b, float a = 1);

        void clear();

        bool operator==(const Color& rhs) const;
        bool operator!=(const Color& rhs) const;

    public:
        float r;
        float g;
        float b;
        float a;
    };
}

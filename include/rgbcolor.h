#ifndef __RGBCOLOR_H__
#define __RGBCOLOR_H__

struct RGBColor
{
    int r, g, b;
    float alpha;
    RGBColor() : r(0), g(0), b(0), alpha(1.0f) {}
    RGBColor(int r, int g, int b, float alpha = 1.0f) : r(r), g(g), b(b), alpha(alpha) {}
};

#endif
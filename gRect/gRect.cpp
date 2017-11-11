#include "gRect.h"
using namespace gfw;
gRect::gRect()
{
    top=0;
    bottom=0;
    left=0;
    right=0;
}


gRect::gRect(const gRect &other)
{
    top=other.top;
    bottom=other.bottom;
    left=other.left;
    right=other.right;
}


gRect::gRect(int _top, int _bottom, int _left, int _right)
{
    top=_top;
    bottom=_bottom;
    left=_left;
    right=_right;
}

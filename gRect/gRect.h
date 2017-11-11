#ifndef GRECT_H
#define GRECT_H

#include <gfw_global.h>
namespace gfw
{
    struct SHARED_GFW gRect
    {
        int top;
        int bottom;
        int left;
        int right;
        gRect();
        gRect(const gRect &other);
        gRect(int _top,int _bottom,int _left,int _right);
        inline int width() const
        {
            return right - left;
        }

        inline int height() const
        {
            return bottom - top;
        }
        inline bool isZero() const
        {
            return right==0 && left ==0 && top==0 && bottom==0;
        }
        inline void create(int _top, int _bottom, int _left, int _right)
        {
            top=_top;bottom=_bottom;
            left=_left;right=_right;
        }

    };

    struct gPoint2D
    {
        int x;
        int y;
        gPoint2D()
        {
            x=0;
            y=0;
        }
        gPoint2D(const gPoint2D &other)
        {
            x=other.x;
            y=other.y;
        }

        gPoint2D &operator=(const gPoint2D &other)
        {
            x=other.x;
            y=other.y;
            return *this;
        }
    };

    template <class T>
    class gtRect
    {
    private:
        T m_top;
        T m_bottom;
        T m_left;
        T m_right;
    public:
        inline const T &top() const
        {
            return m_top;
        }
        inline const T &bottom() const
        {
            return m_bottom;
        }
        inline const T &left() const
        {
            return m_left;
        }
        inline const T &right() const
        {
            return m_right;
        }
        inline T &top()
        {
            return m_top;
        }
        inline T &bottom()
        {
            return m_bottom;
        }
        inline T &left()
        {
            return m_left;
        }
        inline T &right()
        {
            return m_right;
        }
        gtRect():m_top(0),m_bottom(0),m_left(0),m_right(0)
        {

        }
        gtRect(const gtRect<T> &other)
        {
            m_top=other.top();
            m_bottom=other.bottom();
            m_left=other.left();
            m_right=other.right();
        }

        gtRect(const T &_top,const T &_bottom,const T &_right,const T &_left)
        {
            m_top=_top;
            m_bottom=_bottom;
            m_right=_right;
            m_left=_left;
        }

        inline void setTop(const T &value)
        {
            m_top=value;
        }
        inline void setBottom(const T &value)
        {
            m_bottom=value;
        }
        inline void setLeft(const T &value)
        {
            m_left=value;
        }
        inline void setRight(const T &value)
        {
            m_right=value;
        }

    };

    typedef gtRect<gPoint2D> g2DPointRect;
}
#endif // GRECT_H

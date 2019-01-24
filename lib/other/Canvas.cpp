//-------------------------------------
// Copyright Reliable Software (c) 1999
//-------------------------------------

#include "canvas.h"
#include "tools.h"
#include <cassert>

void Canvas::Frame (RECT const & rect, HBRUSH brush)
{
	::FrameRect (_hdc, &rect, brush);
}

void Canvas::FillRect (RECT const & rect, HBRUSH brush)
{
    ::FillRect (_hdc, &rect, brush);
}


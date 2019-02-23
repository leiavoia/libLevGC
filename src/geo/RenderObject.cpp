#include "RenderObject.h"
#include "../core/RenderingLayer.h"

namespace LGC {

// as a safety precaution, attempt to unregister self from drawing
RenderObject::~RenderObject() {
	Renderer->Unregister( this );
	}

} // end namespace LGC






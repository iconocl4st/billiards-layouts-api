//
// Created by thallock on 11/10/21.
//

#ifndef IDEA_LAYOUT_MANAGER_IMPL_H
#define IDEA_LAYOUT_MANAGER_IMPL_H

#define USE_UNQL 1

#if USE_UNQL
#include "UnqlManager.h"
#else
#include "FileLayoutManager.h"
#endif

namespace billiards::layout {
#if USE_UNQL
	typedef UnqlLayoutManager LayoutsManagerImpl;
#else
	typedef FileLayoutManager LayoutsManagerImpl;
#endif
}

#endif // IDEA_LAYOUT_MANAGER_IMPL_H
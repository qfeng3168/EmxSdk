//
// Created by xiong on 2021/7/19.
//

#ifndef EMX_EmxCore_HPP
#define EMX_EmxCore_HPP

#include "core/EmxTypeDef.hpp"
#include "core/EmxLog.hpp"
#include "core/EmxUtils.hpp"
#include "core/EmxGpio.hpp"
#include "core/EmxUV.hpp"
#include "core/EmxMsg.hpp"
#include "core/EmxParam.hpp"
#ifdef __mips__
#define EP_MALLOC(size) malloc(size);
#else
#define EP_MALLOC(size) aligned_alloc(64,size);
#endif
#endif //EMX_EmxCore_HPP


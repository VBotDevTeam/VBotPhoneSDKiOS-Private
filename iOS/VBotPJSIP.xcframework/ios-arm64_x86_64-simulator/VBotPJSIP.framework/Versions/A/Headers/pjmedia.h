/* 
 * Copyright (C) 2008-2011 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#ifndef __PJMEDIA_H__
#define __PJMEDIA_H__

/**
 * @file pjmedia.h
 * @brief PJMEDIA main header file.
 */
#include <VBotPJSIP/pjmedia/alaw_ulaw.h>
#include <VBotPJSIP/pjmedia/avi_stream.h>
#include <VBotPJSIP/pjmedia/bidirectional.h>
#include <VBotPJSIP/pjmedia/circbuf.h>
#include <VBotPJSIP/pjmedia/clock.h>
#include <VBotPJSIP/pjmedia/codec.h>
#include <VBotPJSIP/pjmedia/conference.h>
#include <VBotPJSIP/pjmedia/converter.h>
#include <VBotPJSIP/pjmedia/delaybuf.h>
#include <VBotPJSIP/pjmedia/echo.h>
#include <VBotPJSIP/pjmedia/echo_port.h>
#include <VBotPJSIP/pjmedia/endpoint.h>
#include <VBotPJSIP/pjmedia/errno.h>
#include <VBotPJSIP/pjmedia/event.h>
#include <VBotPJSIP/pjmedia/frame.h>
#include <VBotPJSIP/pjmedia/format.h>
#include <VBotPJSIP/pjmedia/g711.h>
#include <VBotPJSIP/pjmedia/jbuf.h>
#include <VBotPJSIP/pjmedia/master_port.h>
#include <VBotPJSIP/pjmedia/mem_port.h>
#include <VBotPJSIP/pjmedia/null_port.h>
#include <VBotPJSIP/pjmedia/plc.h>
#include <VBotPJSIP/pjmedia/port.h>
#include <VBotPJSIP/pjmedia/resample.h>
#include <VBotPJSIP/pjmedia/rtcp.h>
#include <VBotPJSIP/pjmedia/rtcp_xr.h>
#include <VBotPJSIP/pjmedia/rtp.h>
#include <VBotPJSIP/pjmedia/sdp.h>
#include <VBotPJSIP/pjmedia/sdp_neg.h>
//#include <VBotPJSIP/pjmedia/session.h>
#include <VBotPJSIP/pjmedia/silencedet.h>
#include <VBotPJSIP/pjmedia/sound.h>
#include <VBotPJSIP/pjmedia/sound_port.h>
#include <VBotPJSIP/pjmedia/splitcomb.h>
#include <VBotPJSIP/pjmedia/stereo.h>
#include <VBotPJSIP/pjmedia/stream.h>
#include <VBotPJSIP/pjmedia/stream_common.h>
#include <VBotPJSIP/pjmedia/tonegen.h>
#include <VBotPJSIP/pjmedia/transport.h>
#include <VBotPJSIP/pjmedia/transport_adapter_sample.h>
#include <VBotPJSIP/pjmedia/transport_ice.h>
#include <VBotPJSIP/pjmedia/transport_loop.h>
#include <VBotPJSIP/pjmedia/transport_srtp.h>
#include <VBotPJSIP/pjmedia/transport_udp.h>
#include <VBotPJSIP/pjmedia/vid_codec.h>
#include <VBotPJSIP/pjmedia/vid_conf.h>
#include <VBotPJSIP/pjmedia/vid_port.h>
#include <VBotPJSIP/pjmedia/vid_stream.h>
//#include <VBotPJSIP/pjmedia/vid_tee.h>
#include <VBotPJSIP/pjmedia/wav_playlist.h>
#include <VBotPJSIP/pjmedia/wav_port.h>
#include <VBotPJSIP/pjmedia/wave.h>
#include <VBotPJSIP/pjmedia/wsola.h>

#endif  /* __PJMEDIA_H__ */


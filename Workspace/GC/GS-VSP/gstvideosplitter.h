/* GStreamer
 * Copyright (C) <1999> Erik Walthinsen <omega@cse.ogi.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef __GST_VIDEOSPLITTER_H__
#define __GST_VIDEOSPLITTER_H__

#include <gst/gst.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define GST_TYPE_VIDEOSPLITTER            (gst_videosplitter_get_type())
#define GST_VIDEOSPLITTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_VIDEOSPLITTER,GstVideoSplitter))
#define GST_VIDEOSPLITTER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_VIDEOSPLITTER,GstVideoSplitterClass))
#define GST_IS_VIDEOSPLITTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_VIDEOSPLITTER))
#define GST_IS_VIDEOSPLITTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_VIDEOSPLITTER))

typedef struct _GstVideoSplitter GstVideoSplitter;
typedef struct _GstVideoSplitterClass GstVideoSplitterClass;

struct _GstVideoSplitter
{
  GstElement element;
  GstPad *sinkpad, *srcpad1, *srcpad2;
  gint width;
  gint height;
  gboolean pushsrc2; //If this is false, data will not be pushed over src2

};

struct _GstVideoSplitterClass
{
  GstElementClass parent_class;
};

GType gst_videosplitter_get_type(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#define VERSION "0.10.0.1"
#define PACKAGE "gst-videosplitter"
#define GST_LICENSE "LGPL" //’ undeclared here (not in a function)
#define GST_PACKAGE_NAME "VideoSplitter" //’ undeclared here (not in a function)
#define GST_PACKAGE_ORIGIN "http://sreedal.googlepages.com" //’ undeclared here (not in a function)


#endif /* __GST_VIDEOSPLITTER_H__ */
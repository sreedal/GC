#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <string.h>
#include <gst/gst.h>
#include "gstvideosplitter.h"
#include <gst/video/video.h>



static const GstElementDetails gst_videosplitter_details =
GST_ELEMENT_DETAILS ("VideoSplitter",
    "Filter",
    "Splits an incoming video stream into two identical streams",
    "Senthil Kumar <senthil@alcatel-lucent.com>");


GST_DEBUG_CATEGORY_STATIC (videosplitter_debug);
#define GST_CAT_DEFAULT videosplitter_debug

/* Filter signals and args */
enum
{
  /* FILL ME */
  LAST_SIGNAL
};



enum
{
  ARG_0,
  ARG_PUSHSRC2,
};

static GstStaticPadTemplate videosplitter_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
	GST_STATIC_CAPS (GST_VIDEO_CAPS_BGRx ";" GST_VIDEO_CAPS_RGBx)
    );

static GstStaticPadTemplate videosplitter_src_template1 =
GST_STATIC_PAD_TEMPLATE ("src1",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_BGRx ";" GST_VIDEO_CAPS_RGBx)
    );

static GstStaticPadTemplate videosplitter_src_template2 =
GST_STATIC_PAD_TEMPLATE ("src2",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS (GST_VIDEO_CAPS_BGRx ";" GST_VIDEO_CAPS_RGBx)
    );


GST_BOILERPLATE (GstVideoSplitter, gst_videosplitter, GstElement, GST_TYPE_ELEMENT);

static void gst_videosplitter_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec);
static void gst_videosplitter_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec);

static GstFlowReturn gst_videosplitter_chain (GstPad * pad, GstBuffer * data);

 

static void
gst_videosplitter_base_init (gpointer g_class)
{
  GstElementClass *element_class = GST_ELEMENT_CLASS (g_class);

  gst_element_class_add_pad_template
      (element_class, gst_static_pad_template_get (&videosplitter_sink_template));
  gst_element_class_add_pad_template
      (element_class, gst_static_pad_template_get (&videosplitter_src_template1));
  gst_element_class_add_pad_template
      (element_class, gst_static_pad_template_get (&videosplitter_src_template2));

  gst_element_class_set_details (element_class, &gst_videosplitter_details);
}


static void
gst_videosplitter_finalize (GObject * object)
{
  GstVideoSplitter *videosplitter = GST_VIDEOSPLITTER (object);

  
  G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gst_videosplitter_class_init (GstVideoSplitterClass * klass)
{
  GObjectClass *gobject_class;

  gobject_class = (GObjectClass *) klass;

  parent_class = g_type_class_peek_parent (klass);

  gobject_class->get_property = gst_videosplitter_get_property;
  gobject_class->set_property = gst_videosplitter_set_property;

  g_object_class_install_property (gobject_class, ARG_PUSHSRC2,
      g_param_spec_boolean ("pushsrc2", "pushsrc2",
          "If this is false, data won't be pushed on scr2",
          TRUE, (GParamFlags) G_PARAM_READWRITE));

  gobject_class->finalize = gst_videosplitter_finalize;
  GST_DEBUG_CATEGORY_INIT (videosplitter_debug, "videosplitter", 0, "video splitter");
}


static gboolean
gst_videosplitter_setcaps (GstPad * pad, GstCaps * caps)
{
  GstVideoSplitter *videosplitter;
  const GValue *fps;
  GstStructure *structure;
  //GstCaps *pcaps;
  gboolean ret = TRUE;

  videosplitter = GST_VIDEOSPLITTER (gst_pad_get_parent (pad));

  structure = gst_caps_get_structure (caps, 0);
  gst_structure_get_int (structure, "width", &videosplitter->width);
  gst_structure_get_int (structure, "height", &videosplitter->height);
  fps = gst_structure_get_value (structure, "framerate");

  //ret = gst_pad_set_caps (videosplitter->srcpad1, caps); //copy the caps from the sink to the source
  //ret = gst_pad_set_caps (videosplitter->srcpad2, caps);

  
  gst_object_unref (videosplitter);

  return ret;
}

static void
gst_videosplitter_init (GstVideoSplitter * videosplitter, GstVideoSplitterClass * g_class)
{
  /* sinkpad */
  videosplitter->sinkpad = gst_pad_new_from_static_template
      (&videosplitter_sink_template, "sink");
  gst_pad_set_chain_function (videosplitter->sinkpad, gst_videosplitter_chain);
  gst_pad_set_setcaps_function (videosplitter->sinkpad, gst_videosplitter_setcaps);
  gst_element_add_pad (GST_ELEMENT (videosplitter), videosplitter->sinkpad);

  /* srcpad1 */
  videosplitter->srcpad1 = gst_pad_new_from_static_template
      (&videosplitter_src_template1, "src1");
  gst_element_add_pad (GST_ELEMENT (videosplitter), videosplitter->srcpad1);

  /* srcpad2 */
  videosplitter->srcpad2 = gst_pad_new_from_static_template
      (&videosplitter_src_template2, "src2");

  gst_element_add_pad (GST_ELEMENT (videosplitter), videosplitter->srcpad2);

 videosplitter->pushsrc2 = TRUE;
}



static GstFlowReturn
gst_videosplitter_chain (GstPad * pad, GstBuffer * buf)
{
  GstVideoSplitter *videosplitter;
  GstFlowReturn ret = GST_FLOW_OK;
  GstBuffer *outbuf1 = NULL;
  GstBuffer *outbuf2 = NULL;
  gint width, height;	
 
  
  printf("\n==========In gst_videosplitter_chain==========\n");
  videosplitter = GST_VIDEOSPLITTER (gst_pad_get_parent (pad));
  width = videosplitter->width;
  height = videosplitter->height;


  outbuf1 = gst_buffer_copy(buf); //Create outbuf1 as a copy of buf.
  gst_buffer_copy_metadata(outbuf1, buf, GST_BUFFER_COPY_ALL); 
  //GST_BUFFER_TIMESTAMP(outbuf1) = GST_CLOCK_TIME_NONE;//timestampOut;

  printf("\n==========Pushing to src1==========\n");
  ret = gst_pad_push (videosplitter->srcpad1, outbuf1);
  if( ret != GST_FLOW_OK) printf("\n===========Push Failed============\n");
  if(ret != GST_FLOW_OK) goto push_failed;
  printf("\n==========Push successful==========\n");


 if(videosplitter->pushsrc2) //Create outbuf2 and push it onto srcpad2 only if videosplitter->pushsrc2 is TRUE.
 {
	  outbuf2 = gst_buffer_copy(buf); //Create outbuf2 as a copy of buf
	  gst_buffer_copy_metadata(outbuf2, buf, GST_BUFFER_COPY_ALL);
	  //GST_BUFFER_TIMESTAMP(outbuf2) = GST_CLOCK_TIME_NONE;//timestampOut;

	  printf("\n==========Pushing to src2==========\n");
	  ret = gst_pad_push (videosplitter->srcpad2, outbuf2);
	  printf("\n==========Push successful==========\n");  
 }

push_failed:
  if(ret != GST_FLOW_OK)
  {
	  GST_DEBUG ("push() failed, flow = %s", gst_flow_get_name (ret));
  }

  
  gst_buffer_unref (buf);
  gst_object_unref (videosplitter);
 
  return ret;
}


static void
gst_videosplitter_get_property (GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec)
{
  GstVideoSplitter *videosplitter;

  videosplitter = GST_VIDEOSPLITTER (object);

  switch (prop_id) {
	case ARG_PUSHSRC2:
      g_value_set_boolean (value, videosplitter->pushsrc2);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}


static void
gst_videosplitter_set_property (GObject * object,
    guint prop_id, const GValue * value, GParamSpec * pspec)
{
  GstVideoSplitter *videosplitter;

  videosplitter = GST_VIDEOSPLITTER (object);

  switch (prop_id) {
	case ARG_PUSHSRC2:
      videosplitter->pushsrc2 = g_value_get_boolean (value);
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}




static gboolean
plugin_init (GstPlugin * plugin)
{
  
  if (!gst_element_register (plugin, "videosplitter", GST_RANK_PRIMARY,
          GST_TYPE_VIDEOSPLITTER))
    return FALSE;

  return TRUE;
}


GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    "videosplitter",
    "Splits incoming video stream into two identical streams",
    plugin_init, VERSION, GST_LICENSE, GST_PACKAGE_NAME, GST_PACKAGE_ORIGIN);

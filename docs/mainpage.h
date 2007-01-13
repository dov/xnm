/*! @mainpage 

  @section intro Introduction

  libxnm is a C library for parsing the XNM format, which features:

  - A bison based parser for the xnm syntax.

  - A reference count handled structure XnmValue that can contain
    either a string, an array, or a table.

  - Retriever utility functions for easy retrieval of data from a
    XnmValue tree.

  \section API API documentation

  - \link xnm.h xnm.h \endlink
  
  \section Examples Examples
  Here are a few examples of some uses of libxnm.

  \subsection conf_parsing simple configuration parsing

  Given a xnm file that looks like:
\code
fullscreen: T
audio_rate: 44100
stereo: F
player: Player
fps: 30
\endcode

  Here is code to parse this file:
\code
#int <xnm.h>

int main()
{
  XnmValue *cnf;

  // Configuration default values
  gboolean fullscreen = FALSE;
  gdouble audio_rate = 44100;
  gboolean stereo = FALSE;
  const gchar player = "Joe";
  gint fps = 15;
  gboolean reflection = TRUE;


  if (xnm_parse_file(cnf_file,
                     // output
                     &cnf,
                     &error) != 0)
      die("Failed reading file.\n");

  ret = xnm_value_get_values(val,
                            "fullscreen", XNM_GET_BOOL, &fullscreen,
                            "audio_rate", XNM_GET_DOUBLE, &audio_root,
                            "stereo", XNM_GET_BOOL, &stereo,
                            "player", XNM_GET_STRING, &player,
                            "fps", XNM_GET_INT,  &fps,
                             NULL);
}
\endcode

\subsection gaim_like Gaim prefs like
\code
core => {
  away => {
    idle_reporting=>system
    away_when_idle=>0
    mins_before_way=>10
    auto_reply=>awayidle
    report_idle=>1
  }
  buddies => {}
  contact => {
    last_match => 0
    offline_score => 2
    away_score => 2
    idle_score => 1
  }
  gaim => {
    gtk=> {
      browsers=> {
        place=>F
	command=>"xterm -e lynx %s"
	browser=>firefox
	new_window=>F
      }
      plugins => [
        '/usr/lib/gaim/gaimrc.so'
	'/usr/lib/gaim/ssl-nss.so'
	'/usr/lib/gaim/ssl.so'
      ]
    }
  }
}
\endcode

As a short example, let's just iterate over the plugins:

\code
    XnmValue *xnm_root, *xnm_plugins;

    /* Retrieve a shortcut. */
    xnm_value_get(xnm_root,
                  "core/gaim/gtk/plugins",
                  &xnm_plugins);

    int num_plugins = xnm_value_get_array_length(xnm_plugins, "");

    for (i=0; i<num_plugins; i++)
      {
        const gchar *plugin_name;
        gchar key[16];
        sprintf(key, "[%d]", i);
        xnm_value_get_const_string(key, &plugin_name);
      }

    xnm_value_unref(xnm_plugins);
}

\endcode
*/

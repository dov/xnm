/* libxnm
 * xnm-retrieve.c:
 *
 * Copyright (C) 2007 Dov Grobgeld
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xnm.h"
#include "xnm_private.h"


int xnm_value_get (XnmValue *xnm_value_tree,
                   const char *key,
                   // output
                   XnmValue **xnm_value)
{
  int ret = 0;
  char *key_head, *key_tail;
  gboolean is_leaf, is_array;

  // Check if it is an endnode
  if (xnm_value_tree->type == XNM_STRING)
    {
      if (strlen(key) == 0)
        {
          *xnm_value = xnm_value_tree;
          xnm_value_ref(*xnm_value);
          return 0;
        }
      else
        return -1;
    }

  // Interpret and recurse
  xnm_key_split(key,
                &is_leaf,
                &is_array,
                &key_head,
                &key_tail
                );
#if 0
  printf("head tail is_leaf is_array = %s %s %d %d\n", key_head, key_tail, is_leaf, is_array);
#endif
  
  if ((xnm_value_tree->type == XNM_ARRAY && !is_array)
      || (xnm_value_tree->type != XNM_ARRAY && is_array))
    return -1;

  if (xnm_value_tree->type == XNM_ARRAY)
    {
      XnmValue *xnm_value_array;
      int idx = atoi(key_head);
      int ret = xnm_array_get_index(xnm_value_tree->value.array,
                                    idx,
                                    // output
                                    &xnm_value_array);
      if (ret!= 0)
        ;

      else if (is_leaf)
        *xnm_value = xnm_value_array;
      else
        {
          ret = xnm_value_get(xnm_value_array,
                              key_tail,
                              // output
                              xnm_value);
          xnm_value_unref(xnm_value_array);
        }
    }
  else if (xnm_value_tree->type == XNM_TABLE)
    {
      XnmValue *xnm_value_table;

      ret = xnm_table_get_key(xnm_value_tree->value.table,
                              key_head,
                              // output
                              &xnm_value_table);

      if (ret != 0)
        ;
      else if (is_leaf)
          *xnm_value = xnm_value_table;
      else
        {
          ret = xnm_value_get(xnm_value_table,
                              key_tail,
                              // output
                              xnm_value);
          xnm_value_unref(xnm_value_table);
        }
    }

  g_free(key_head);
  g_free(key_tail);

  return ret;
}

int
xnm_value_get_string       (XnmValue *xnm_value_tree,
                            const char *key,
                            // output
                            gchar **string_val)
{
  int ret = 0;
  XnmValue *xnm_value_string;
  ret = xnm_value_get(xnm_value_tree,
                      key,
                      /* output */
                      &xnm_value_string);
  if (ret != 0)
    return ret;

  if (xnm_value_string->type == XNM_STRING)
    *string_val = g_strdup(xnm_value_string->value.string->string);
  else
    {
      GString *gs = g_string_new("");
      gchar *serialize = xnm_value_export_to_string(xnm_value_string);
      if (xnm_value_string->type == XNM_ARRAY)
        {
          g_string_append(gs,"[");
          g_string_append(gs,serialize);
          g_string_append(gs,"]");
        }
      else
        {
          g_string_append(gs,"{");
          g_string_append(gs,serialize);
          g_string_append(gs,"}");
        }
      g_free(serialize);
      *string_val = gs->str;
      g_string_free(gs, FALSE);
    }

  xnm_value_unref(xnm_value_string);
  return ret;
}

int
xnm_value_get_const_string       (XnmValue *xnm_value_tree,
                                  const char *key,
                                  // output
                                  const gchar **string_val)
{
  int ret = 0;
  XnmValue *xnm_value_string;
  ret = xnm_value_get(xnm_value_tree,
                      key,
                      /* output */
                      &xnm_value_string);
  if (ret != 0)
    return ret;

  if (xnm_value_string->type == XNM_STRING)
    *string_val = xnm_value_string->value.string->string;
  else
    return -1;

  return ret;
}

int xnm_value_get_int (XnmValue *xnm_value,
                       const char *key,
                       /* output */
                       int *val_int)
{
  int ret;
  XnmValue *xnm_value_int;
  ret = xnm_value_get(xnm_value,
                      key,
                      /* output */
                      &xnm_value_int);
  if (ret != 0)
    return ret;

  // Should this return an error??
  if (xnm_value_int->type != XNM_STRING)
    return -1;
  
  *val_int = atoi(xnm_value_int->value.string->string);

  xnm_value_unref(xnm_value_int);

  return ret;
}

int xnm_value_get_double (XnmValue *xnm_value,
                          const char *key,
                          /* output */
                          double *val_double)
{
  int ret;
  XnmValue *xnm_value_double;
  ret = xnm_value_get(xnm_value,
                      key,
                      /* output */
                      &xnm_value_double);
  if (ret != 0)
    return ret;

  // Should this return an error??
  if (xnm_value_double->type != XNM_STRING)
    return -1;
  
  *val_double = atof(xnm_value_double->value.string->string);

  xnm_value_unref(xnm_value_double);

  return ret;
}

int
xnm_value_get_array_length (XnmValue *xnm_value,
                            const char *key)
{
  int ret;
  XnmValue *xnm_value_array;

  ret = xnm_value_get(xnm_value,
                      key,
                      /* output */
                      &xnm_value_array);

  if (ret != 0)
    return ret;
  
  if (!xnm_value_array || xnm_value_array->type != XNM_ARRAY)
    return -1;

  ret = xnm_value_array->value.array->array->len;

  xnm_value_unref(xnm_value_array);
  return 0;
}

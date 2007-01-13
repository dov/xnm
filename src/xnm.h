/* libxnm
 * xnm.h:
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
 *
 * Dov Grobgeld <dov.grobgeld@gmail.com>
 * Thu Jan 11 23:44:32 2007
 */

/**
 * @file   xnm.h
 * @author Dov Grobgeld <dov.grobgeld@gmail.com>
 * @date   Fri Jan 12 09:23:04 2007
 * 
 * @brief  The libxnm header file.
 * 
 * 
 */

#ifndef XNM_H
#define XNM_H

#include <glib.h>

/**
 * Error definitions
 * 
 */
enum {
  XNM_ERROR = 0,
  XNM_ERROR_SYNTAX_ERROR = 1,
  XNM_ERROR_KEY_NOT_FOUND = 2
};

/**
 * The possible types of a XnmValue.
 * 
 */
typedef enum  {
  XNM_UNKNOWN,
  XNM_TABLE,
  XNM_ARRAY,
  XNM_STRING,
  XNM_BINARY 
} XnmValueType;

/**
 * Values used when retrieving.
 * 
 */
enum {
  XNM_GET_INT,
  XNM_GET_DOUBLE,
  XNM_GET_STRING,
  XNM_GET_BOOL
};

/* Forward declarations */
struct xnm_string_struct;
struct xnm_table_struct;
struct xnm_array_struct;

/**
 * The xnmValue should be considered an opaque reference.
 * 
 */
typedef struct {
    int ref_count;
    XnmValueType type;

    union {
	struct xnm_table_struct *table;
	struct xnm_array_struct *array;
	struct xnm_string_struct *string;
    } value;
} XnmValue;

/** 
 * Allocate a new XnmValue with the type string. The string is initialized
 * to a copy of s.
 * 
 * @param s 
 * 
 * @return Newly allocated XnmValue
 */
XnmValue * xnm_value_new_string            (const gchar *s);

/** 
 * Allocate a new XnmValue with the type XNM_TABLE.
 * 
 * 
 * @return 
 */
XnmValue * xnm_value_new_table             ();

/** 
 * Allocate a new XnmValue with the type XNM_ARRAY.
 * 
 * @return 
 */
XnmValue * xnm_value_new_array             ();

/** 
 * Increase the reference count of xnm_value.
 * 
 * @param xnm_value 
 */
void          xnm_value_ref              (XnmValue *xnm_value);

/** 
 * Decrease the reference count on an xnm_value. If the reference
 * count goes down to 0, the value is destroyed.
 * 
 * @param xnm_value 
 */
void          xnm_value_unref            (XnmValue *xnm_value);

/** 
 * Allocate a new string containing a serialized representation
 * of xnm_value.
 * 
 * @param xnm_value 
 * 
 * @return 
 */
gchar *       xnm_value_export_to_string (XnmValue *xnm_value);

/** 
 * Retrieve an xnm value from an xnm_value_tree. If the key is not
 * found then the value is not assigned. If the key is found then
 * xnm_value is assigned the corresponding string.
 *
 * Note that xnm_value_get does *not* dereference xnm_value before
 * overwriting it with a new value.
 *
 * The returned xnm_value points into the xnm_value_tree with an
 * increased reference count.
 * 
 * @param xnm_value_tree
 * @param key 
 * @param xnm_value 
 * 
 * @return 0 if value retrieved, -1 if value not found, -2 if key syntax error.
 */
int           xnm_value_get              (XnmValue *xnm_value_tree,
					  const char *key,
					  // output
					  XnmValue **xnm_value);

/** 
 * This value retrieves a string from an xnm_value_tree. If the key
 * is found then string_value is freed, and the string is copied
 * into string_val. Otherwise, string_val is left untouched.
 * 
 * @param xnm_value 
 * @param key 
 * @param string_val 
 * 
 * @return 0 if value retrieved, -1 if value not found, -2 if key syntax error.
 */
int           xnm_value_get_string       (XnmValue *xnm_value_tree,
					  const char *key,
					  // output
                                          gchar **string_val);

/** 
 * This value retrieves a const pointer to the string within the
 * xnm_value_tree.
 * 
 * @param xnm_value_tree 
 * @param key 
 * @retval const_val_string 
 * 
 * @return 0 if value retrieved, -1 if value not found, -2 if key syntax error.
 */
int           xnm_value_get_const_string  (XnmValue *xnm_value_tree,
                                           const char *key,

                                           /* output */
                                           const gchar **const_val_string);

/** 
 * This value retrieves a value from an xnm_value_tree and turns it
 * into an integer with atoi().
 * 
 * @param xnm_value_tree 
 * @param key 
 * @retval val_int
 * 
 * @return 0 if value retrieved, -1 if value not found, -2 if key syntax error.
 */
int           xnm_value_get_int           (XnmValue *xnm_value,
                                           const char *key,
                                           /* output */
                                           int *val_int);


/**
 * Retrieve a boolean value from an xnm_value. The boolean value is
 * considered true if it starts with 't','T','y','Y', or 1. Everything
 * else is false.
 */
int xnm_value_get_bool (XnmValue *xnm_value,
                        const char *key,
                        /* output */
                        gboolean *val_bool);

/** 
 * Add an xnm_value into an XnmValue of type array.
 * 
 * @return 
 */
int xnm_value_array_push_value(XnmValue * xnm_value_array,
                               XnmValue * value);

/** 
 * Push a string into an xnm value of type array.
 * 
 * @param xnm_value_array 
 * @param string 
 * 
 * @return 
 */
int xnm_value_array_push_string(XnmValue * xnm_value_array,
                                const gchar *string);


/** 
 * Push a string into an xnm value of type array.
 * 
 * @param xnm_value_array 
 * @param string 
 * 
 * @return 
 */
int xnm_value_array_push_int(XnmValue * xnm_value_array,
                             int value_int);


/** 
 * Formatted addition to an array. Use e.g. for doubles.
 * 
 * @param xnm_value_array 
 * @param format 
 * 
 * @return 
 */
int xnm_value_array_push_printf(XnmValue * xnm_value_array,
                                const gchar *format,
                                ...);

/** 
 * Add a xnm_value to a xnm table. This function increases the reference
 * count of the xnm_value.
 *
 * (TBD: Does this function support hierarchical keys?)
 * 
 * @param xnm_value_parent 
 * @param key 
 * @param xnm_value 
 * 
 * @return 
 */
int xnm_value_table_set_key_value(XnmValue    * xnm_value_tree,
                                  const gchar * key,
                                  XnmValue    * xnm_value);

/** 
 * Convenience function for setting a string key value. If the key
 * already exists in xnm_value_parent, then it is freed and replaced
 * by the new svalue.
 * 
 * @param xnm_value_parent 
 * @param key 
 * @param string 
 * 
 * @return 
 */
int
xnm_value_table_set_key_value_string(XnmValue *xnm_value_parent,
                                     const gchar *key,
                                     const gchar *string);

/** 
 * Set a key value which is formatted with printf.
 * 
 * @param xnm_value_parent 
 * @param key 
 * @param format 
 * 
 * @return 
 */
int
xnm_value_set_key_value_printf(XnmValue *xnm_value_parent,
                               const gchar *key,
                               const gchar *format,
                               ...
                               );

/** 
 * Retrive the length of a XnmValue array.
 * 
 * @param xnm_value 
 * @param key 
 * @param error 
 * 
 * @return Length of array if node found. -1 if key not found.
 */
int
xnm_value_get_array_length (XnmValue *xnm_value,
                            const char *key);

/** 
 * Get a list of values in one call. 
 * 
 * @param xnm_value 
 * 
 * @return 
 */
int
xnm_value_get_values(XnmValue *xnm_value,
                     ...);

/** 
 * Parse a string in XNM syntax.
 * 
 * @param string A string in xnm format
 * @retval value A newly allocated XnmValue if the parsing was successful.
 * @retval error Error if parsing failed.
 * 
 * @return 
 */
int
xnm_parse_string(const gchar *string,
                 // output
                 XnmValue **value,
                 GError **error
                 );

/** 
 * Parse a string in asf format with a given len.
 * 
 * @param string 
 * @param len 
 * @param value 
 * @param error 
 * 
 * @return 
 */
int
xnm_parse_string_len(const gchar *string,
                     int len,
                     // output
                     XnmValue **value,
                     GError **error
                     );


/** 
 * Read a file and parse it.
 * 
 * @param filenam 
 * @param xnm_value 
 * @param error 
 * 
 * @return 
 */
int
xnm_parse_file(const gchar *filenam,
               // output
               XnmValue **xnm_value,
               GError **error);

#endif

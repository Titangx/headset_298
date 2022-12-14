/*!
\copyright  Copyright (c) 2018-2022 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\file
\brief      Header file for a data structure with a list of { key, value } elements.

*/
#ifndef KEY_VALUE_LIST_H
#define KEY_VALUE_LIST_H

#include <stdlib.h>

/*! \brief Opaque type for a key-value list object. */
typedef struct key_value_list_tag * key_value_list_t;

/*! \brief The key type is a 16bit unsigned integer. */
typedef uint16 key_value_key_t;

/*! \brief Callback function type for key value list merge resolving */
typedef bool (*key_value_list_merge_resolve_callback_t)(key_value_list_t source, key_value_list_t target, key_value_key_t key);

/*! \brief Callback function type which needs to get called when new key value pair gets added to a list */
typedef void (*key_value_list_add_callback_t)(key_value_list_t list, key_value_key_t key, const void *value, size_t size);

/*! \brief Create a key-value list.

    Creates an empty key-value list and returns an opaque
    handle to it.

    \return A handle to a key-value list or 0 if it failed.
*/
key_value_list_t KeyValueList_Create(void);

/*! \brief Destroy a key-value list.

    If #list contains any key-value pairs they will all be removed and
    any memory they own will be freed.

    \param list Key-value list to destroy.
*/
void KeyValueList_Destroy(key_value_list_t* list);

/*! \brief Add a key-value pair to a key-value list.

    This function will add the new key-value pair to the given list unless the
    key is already in the list or the list is full.

    The data passed in via #value is copied to the list and if necessary a new
    buffer will be allocated and owned by the list to store the copied data.
    If the buffer cannot be allocated this function will panic.

    Small values of 4 octets or less will not require a buffer to be allocated
    by the list to store the copied data.

    \param list Key-value list to add to.
    \param key Key to insert.
    \param value Pointer to the data to associate with the key.
    \param size Size in octets of the data pointed to by value.

    \return bool TRUE if key-value was added successfully; FALSE otherwise.
*/
bool KeyValueList_Add(key_value_list_t list, key_value_key_t key, const void *value, size_t size);

/*! \brief Remove a key-value pair from a key-value list.

    Any memory owned by the key-value pair will be freed when it is removed.

    If the #key is not in the list this function will not change
    the #list and will not raise any error or panic.

    \param list Key-value list to remove the key-value pair from.
    \param key Key to be removed.
*/
void KeyValueList_Remove(key_value_list_t list, key_value_key_t key);

/*! \brief Remove all key-value pairs from a key-value list.

    Any memory owned by a key-value pair will be freed when it is removed.

    \param list Key-value list to remove all key-value pairs from.
*/
void KeyValueList_RemoveAll(key_value_list_t list);

/*! \brief Check if a key is set in a key-value list.

    \param list Key-value list to search in.
    \param key Key to search for.

    \return bool TRUE if key exists; FALSE otherwise.
*/
bool KeyValueList_IsSet(key_value_list_t list, key_value_key_t key);

/*! \brief Get the value for a key in a key-value list.

    The value is returned as a pointer to the buffer owned by the key-value
    pair and the size of the buffer.

    If the key does not exist the pointer to the buffer and the pointer to
    the size will not be modified.

    \param[in] list Key-value list to search in.
    \param[in] key Key to search for.
    \param[out] value The pointer to the key value is written into this.
    \param[out] size The size of the key value is written into this.

    \return TRUE if the key exists, FALSE otherwise.
*/
bool KeyValueList_Get(key_value_list_t list, key_value_key_t key, void **value, size_t *size);

/*! \brief Get the value (with an expected size) for a key in a key-value list.

    The value is returned as a pointer to the buffer owned by the key-value
    pair.

    If the key does not exist the pointer to the buffer will not be modified.

    The function will panic if an item with the same key is found with a size
    different to the size specified.

    \param[in] list Key-value list to search in.
    \param[in] key Key to search for.
    \param[in] size The expected size of the key value.

    \return value The pointer to the key's value, or NULL is not found.

    \note This function should be used in preference to KeyValueList_Get if
    the size of the object is known.
*/
void *KeyValueList_GetSized(key_value_list_t list, key_value_key_t key, size_t size);

/*! \brief Merge source key-value list with the target key-value list

    This function will merge all the key value pairs from source list to target list.
    If same key exists in target list too, then it will compare the value associated with the key.
    If the value associated are different, then the conflicts needs to be resolved using 'resolve_callback'.
    Panic will happen if resolve fails.

    \param source_list Source Key-value list
    \param target_list Target Key-value list
    \param resolve_callback Callback function to resolve the conflicts.
    The callback function needs to return TRUE if it is able to resolve.
    If it is returned FALSE, then we will panic.
    If NULL is passed as resolve_callback, then we will panic if conflicts happen.

    \return TRUE if success, FALSE otherwise
*/
bool KeyValueList_Merge(key_value_list_t source_list, key_value_list_t target_list,
                        key_value_list_merge_resolve_callback_t resolve_callback);

/*! \brief Register a callback which will gets called when a new key value pair gets added to a list.

    \param key_value_list_add_callback Callback function
*/
void KeyValueList_RegisterAddCallback(key_value_list_add_callback_t callback);

#endif // KEY_VALUE_LIST_H

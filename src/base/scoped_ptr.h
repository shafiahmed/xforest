//------------------------------------------------------------------------------
// Copyright (c) 2018 by contributors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

/*!
 *  Copyright (c) 2018 by Contributors
 * \file scoped_ptr.h 
 * \brief This file provides the scoped_ptr and scoped_array classes.
 */
#ifndef XFOREST_BASE_SCOPED_PTR_H_
#define XFOREST_BASE_SCOPED_PTR_H_

#include <assert.h>

//------------------------------------------------------------------------------
// from google3/base/scoped_ptr.h
//
//  This is an implementation designed to match the anticipated future TR2
//  implementation of the scoped_ptr class, and its closely-related brethren,
//  scoped_array, scoped_ptr_malloc, and make_scoped_ptr.
//------------------------------------------------------------------------------

template <class C> class scoped_ptr;
template <class C> class scoped_array;

/*!
 * \brief A scoped_ptr<T> is like a T*, except that the destructor of scoped_ptr<T>
 * automatically deletes the pointer it holds (if any).
 * That is, scoped_ptr<T> owns the T object that it points to.
 * Like a T*, a scoped_ptr<T> may hold either NULL or a pointer to a T object.
 *
 * The size of a scoped_ptr is small:
 * sizeof(scoped_ptr<C>) == sizeof(C*)
 */
template <class C>
class scoped_ptr {
 public:
  // The element type
  typedef C element_type;

  /*!
   * \brief Constructor. Defaults to intializing with nullptr.
   * There is no way to create an uninitialized scoped_ptr.
   * The input parameter must be allocated with new.
   * \param p data pointer
   */
  explicit scoped_ptr(C* p = nullptr) : ptr_(p) { }

  /*!
   * \brief Destructor. If there is a C object, delete it.
   * We don't need to test ptr_ == nullptr because C++ does that for us.
   */
  ~scoped_ptr() {
    enum { type_must_be_complete = sizeof(C) };
    delete ptr_;
  }

  /*!
   * \brief Reset. Deletes the current owned object, if any.
   * Then takes ownership of a new object, if given.
   * this->reset(this->get()) works.
   * \param p data pointer
   */
  void reset(C* p = nullptr) {
    if (p != ptr_) {
      enum { type_must_be_complete = sizeof(C) };
      delete ptr_;
      ptr_ = p;
    }
  }

  /*!
   * \brief Accessors to get the owned object.
   * operator* and operator-> will assert() if there is no current object.
   */
  C& operator*() const {
    assert(ptr_ != nullptr);
    return *ptr_;
  }

  /*!
   * \brief Accessors to get the owned object.
   * operator* and operator-> will assert() if there is no current object.
   */
  C* operator->() const  {
    assert(ptr_ != nullptr);
    return ptr_;
  }

  /*!
   * \brief Accessors to get the owned object.
   * operator* and operator-> will assert() if there is no current object.
   */
  C* get() const { return ptr_; }

  /*!
   * \brief Comparison operators. These return whether two scoped_ptr 
   * refer to the same object, not just to two different but equal objects.
   * \param p data pointer
   * \return true or false
   */
  bool operator==(C* p) const { return ptr_ == p; }

  /*!
   * \brief Comparison operators. These return whether two scoped_ptr 
   * refer to the same object, not just to two different but equal objects.
   * \param p data pointer
   * \return true or false
   */
  bool operator!=(C* p) const { return ptr_ != p; }

  /*!
   * \brief Swap two scoped pointers.
   * \param p2 data refer
   */
  void swap(scoped_ptr& p2) {
    C* tmp = ptr_;
    ptr_ = p2.ptr_;
    p2.ptr_ = tmp;
  }

  /*!
   * Release a pointer. The return value is the current 
   * pointer held by this object. If this object holds a NULL pointer, 
   * the return value is nullptr. After this operation, this object 
   * will hold a NULL pointer, and will not own the object any more.
   * \return current pointer held by this object
   */
  C* release() {
    C* retVal = ptr_;
    ptr_ = nullptr;
    return retVal;
  }

 private:
  C* ptr_;

  /*!
   * \brief Forbid comparison of scoped_ptr types.  If C2 != C, it totally doesn't
   * make sense, and if C2 == C, it still doesn't make sense because you should
   * never have the same object owned by two different scoped_ptrs.
   */
  template <class C2> bool operator==(scoped_ptr<C2> const& p2) const;

  /*!
   * \brief Forbid comparison of scoped_ptr types.  If C2 != C, it totally doesn't
   * make sense, and if C2 == C, it still doesn't make sense because you should
   * never have the same object owned by two different scoped_ptrs.
   */
  template <class C2> bool operator!=(scoped_ptr<C2> const& p2) const;

  /*!
   * \brief Disallow evil constructors
   */
  scoped_ptr(const scoped_ptr&);

  /*!
   * \brief Disallow evil constructors
   */
  void operator=(const scoped_ptr&);
};

/*!
 * \brief scoped_array<C> is like scoped_ptr<C>, except that the caller must allocate
 * with new [] and the destructor deletes objects with delete [].
 *
 * As with scoped_ptr<C>, a scoped_array<C> either points to an object
 * or is NULL. A scoped_array<C> owns the object that it points to.
 *
 * Size: sizeof(scoped_array<C>) == sizeof(C*)
 */
template <class C>
class scoped_array {
 public:

  // The element type
  typedef C element_type;

  /*!
   * \brief Constructor.  Defaults to intializing with NULL.
   * There is no way to create an uninitialized scoped_array.
   * The input parameter must be allocated with new [].
   * \param p data pointer
   */
  explicit scoped_array(C* p = nullptr) : array_(p) { }

  /*!
   * \brief Destructor.  If there is a C object, delete it.
   * We don't need to test ptr_ == NULL because C++ does that for us.
   */
  ~scoped_array() {
    enum { type_must_be_complete = sizeof(C) };
    delete[] array_;
  }

  /*!
   * \brief Reset.  Deletes the current owned object, if any.
   * Then takes ownership of a new object, if given.
   * this->reset(this->get()) works.
   * \param p data pointer
   */
  void reset(C* p = nullptr) {
    if (p != array_) {
      enum { type_must_be_complete = sizeof(C) };
      delete[] array_;
      array_ = p;
    }
  }

  /*!
   * \brief Get one element of the current object.
   * Will assert() if there is no current object, or index i is negative.
   * \param i index
   * \return selected element
   */
  C& operator[](std::ptrdiff_t i) const {
    assert(i >= 0);
    assert(array_ != nullptr);
    return array_[i];
  }

  /*!
   * \brief Get a pointer to the zeroth element of the current object.
   * If there is no current object, return nullptr.
   * \return a pointer to the zeroth element
   */
  C* get() const {
    return array_;
  }

  /*!
   * \brief Comparison operators.
   * These return whether two scoped_array refer to the same object, not just
   * to two different but equal objects.
   */
  bool operator==(C* p) const { return array_ == p; }

  /*!
   * \brief Comparison operators.
   * These return whether two scoped_array refer to the same object, not just
   * to two different but equal objects.
   */
  bool operator!=(C* p) const { return array_ != p; }

  /*!
   * \brief Swap two scoped arrays.
   * \param p2 data refer
   */
  void swap(scoped_array& p2) {
    C* tmp = array_;
    array_ = p2.array_;
    p2.array_ = tmp;
  }

  /*!
   * Release a array. The return value is the current 
   * pointer held by this object. If this object holds a NULL pointer, 
   * the return value is nullptr. After this operation, this object 
   * will hold a NULL pointer, and will not own the object any more.
   * \return current pointer held by this object
   */
  C* release() {
    C* retVal = array_;
    array_ = nullptr;
    return retVal;
  }

 private:
  C* array_;

  /*!
   * \brief Forbid comparison of scoped_array types.  If C2 != C, it totally doesn't
   * make sense, and if C2 == C, it still doesn't make sense because you should
   * never have the same object owned by two different scoped_ptrs.
   */
  template <class C2> bool operator==(scoped_array<C2> const& p2) const;

  /*!
   * \brief Forbid comparison of scoped_array types.  If C2 != C, it totally doesn't
   * make sense, and if C2 == C, it still doesn't make sense because you should
   * never have the same object owned by two different scoped_ptrs.
   */
  template <class C2> bool operator!=(scoped_array<C2> const& p2) const;

  /*!
   * \brief Disallow evil constructors
   */
  scoped_array(const scoped_array&);
  
  /*!
   * \brief Disallow evil constructors
   */
  void operator=(const scoped_array&);
};

#endif  // XFOREST_BASE_SCOPED_PTR_H_

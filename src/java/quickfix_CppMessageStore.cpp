/* ====================================================================
* The QuickFIX Software License, Version 1.0
*
* Copyright (c) 2001 ThoughtWorks, Inc.  All rights
* reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* 3. The end-user documentation included with the redistribution,
*    if any, must include the following acknowledgment:
*       "This product includes software developed by
*        ThoughtWorks, Inc. (http://www.thoughtworks.com/)."
*    Alternately, this acknowledgment may appear in the software itself,
*    if and wherever such third-party acknowledgments normally appear.
*
* 4. The names "QuickFIX" and "ThoughtWorks, Inc." must
*    not be used to endorse or promote products derived from this
*    software without prior written permission. For written
*    permission, please contact quickfix-users@lists.sourceforge.net.
*
* 5. Products derived from this software may not be called "QuickFIX",
*    nor may "QuickFIX" appear in their name, without prior written
*    permission of ThoughtWorks, Inc.
*
* THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED.  IN NO EVENT SHALL THOUGHTWORKS INC OR
* ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
* USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
* ====================================================================
*/

#ifdef _MSC_VER
#include "stdafx.h"
#else
#include "config.h"
#endif

#include "JVM.h"
#include "quickfix_CppMessageStore.h"
#include <quickfix/MessageStore.h>
#include <quickfix/CallStack.h>
#include "Conversions.h"
#include "JavaMessageStore.h"

JNIEXPORT jboolean JNICALL Java_quickfix_CppMessageStore_set
( JNIEnv *pEnv, jobject obj, jint seq, jstring message )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );

  bool threw = false;
  FIX::IOException e;

  const char* umessage = pEnv->GetStringUTFChars( message, 0 );
  bool result = pWrapper->set( seq, umessage, threw, e );
  pEnv->ReleaseStringUTFChars( message, umessage );

  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return 0;
  }
  return result;

  QF_STACK_CATCH
}

JNIEXPORT jboolean JNICALL Java_quickfix_CppMessageStore_get__ILjava_lang_String_2
( JNIEnv *pEnv, jobject obj, jint seq, jstring message )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );

  std::string msg;
  bool threw = false;
  FIX::IOException e;
  bool result = pWrapper->get( seq, msg, threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return 0;
  }

  if ( result )
  {
    message = newString( msg );
    pEnv->DeleteLocalRef( message );
  }
  return result;

  QF_STACK_CATCH
}

JNIEXPORT void JNICALL Java_quickfix_CppMessageStore_get__IILjava_util_Collection_2
( JNIEnv *pEnv, jobject obj, jint start, jint end, jobject array )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  JVMObject jarray( array );

  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  std::vector < std::string > messages;
  bool threw = false;
  FIX::IOException e;
  pWrapper->get( start, end, messages, threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return ;
  }

  jmethodID methodID = jarray.getClass().getMethodID( "add", "(Ljava/lang/Object;)Z" );
  std::vector < std::string > ::iterator i;
  for ( i = messages.begin(); i != messages.end(); ++i )
  {
    jstring jstr = newString( *i );
    pEnv->CallVoidMethod( jarray, methodID, jstr );
    pEnv->DeleteLocalRef( jstr );
  }

  QF_STACK_CATCH
}

JNIEXPORT jint JNICALL Java_quickfix_CppMessageStore_getNextSenderMsgSeqNum
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  jint result = pWrapper->getNextSenderMsgSeqNum( threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return 0;
  }
  return result;

  QF_STACK_CATCH
}

JNIEXPORT jint JNICALL Java_quickfix_CppMessageStore_getNextTargetMsgSeqNum
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  jint result = pWrapper->getNextTargetMsgSeqNum( threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return 0;
  }
  return result;

  QF_STACK_CATCH
}

JNIEXPORT void JNICALL Java_quickfix_CppMessageStore_setNextSenderMsgSeqNum
( JNIEnv *pEnv, jobject obj, jint seq )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  pWrapper->setNextSenderMsgSeqNum( seq, threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return ;
  }

  QF_STACK_CATCH
}

JNIEXPORT void JNICALL Java_quickfix_CppMessageStore_setNextTargetMsgSeqNum
( JNIEnv *pEnv, jobject obj, jint seq )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  pWrapper->setNextTargetMsgSeqNum( seq, threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return ;
  }

  QF_STACK_CATCH
}

JNIEXPORT void JNICALL Java_quickfix_CppMessageStore_incrNextSenderMsgSeqNum
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  pWrapper->incrNextSenderMsgSeqNum( threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return ;
  }

  QF_STACK_CATCH
}

JNIEXPORT void JNICALL Java_quickfix_CppMessageStore_incrNextTargetMsgSeqNum
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  pWrapper->incrNextTargetMsgSeqNum( threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return ;
  }

  QF_STACK_CATCH
}

JNIEXPORT jobject JNICALL Java_quickfix_CppMessageStore_getCreationTime0
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobj( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobj.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  jobject result = newDate( pWrapper->getCreationTime( threw, e ) );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return 0;
  }
  return result;

  QF_STACK_CATCH
}

JNIEXPORT void JNICALL Java_quickfix_CppMessageStore_reset0
( JNIEnv *pEnv, jobject obj )
{ QF_STACK_TRY

  JVM::set( pEnv );
  JVMObject jobject( obj );
  FIX::MessageStoreExceptionWrapper* pWrapper =
    ( FIX::MessageStoreExceptionWrapper* ) jobject.getInt( "cppPointer" );
  bool threw = false;
  FIX::IOException e;
  pWrapper->reset( threw, e );
  if ( threw )
  {
    throwNew( "Ljava/io/IOException;", e.what() );
    return ;
  }

  QF_STACK_CATCH
}
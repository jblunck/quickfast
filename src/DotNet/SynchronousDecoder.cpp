// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include "StdAfx.h"
#include "SynchronousDecoder.h"
#include "Message.h"
#include "TemplateRegistry.h"
#include "Exceptions.h"

using namespace System;
using namespace System::IO;


namespace QuickFASTDotNet{
  namespace Codecs{

    SynchronousDecoder::SynchronousDecoder(TemplateRegistry^ templateRegistry, System::IO::Stream^ instream):
        stream_(instream),
        templateRegistry_(templateRegistry),
        dataSource_(new DataSourceBuffered(instream)),
        decoder_(new QuickFAST::Codecs::Decoder(templateRegistry->NativeTemplateRegistry)),
        resetOnMessage_(false),
        messageCount_(0),
        messageCountLimit_(unsigned int(-1)),
        maxFieldCount_(templateRegistry->MaxFieldCount)
    {

    }



    void SynchronousDecoder::Decode(MessageReceivedDelegate^ callback)
    {
      typedef QuickFASTDotNet::Messages::Message FTManagedMessage;
      bool more = true;
      FTManagedMessage^ message = gcnew FTManagedMessage(maxFieldCount_);
      QuickFAST::Codecs::DataSource & source = dataSource_.GetRef();
      QuickFAST::Messages::DecodedFields & messageRef = message->MessageRef;

      try
      {
        while(more && messageCount_ < messageCountLimit_)
        {
          if(resetOnMessage_)
          {
            decoder_->reset();
          }

          if (! decoder_->decodeMessage(source, messageRef))
          {
            more = false;
            // The c++ version outputs a message to std out
          }
          else
          {
            more |= callback(message);
            messageCount_ += 1;
          }
        }
      }
      catch(const QuickFAST::UnsupportedConversion& error)
      {
        throw gcnew UnsupportedConversion(error);
      }
      catch(const QuickFAST::OverflowError& error)
      {
        throw gcnew OverflowError(error);
      }
      catch(const QuickFAST::FieldNotPresent& error)
      {
        throw gcnew FieldNotPresent(error);
      }
      catch(const QuickFAST::EncodingError& error)
      {
        throw gcnew EncodingError(error);
      }
      catch(const QuickFAST::TemplateDefinitionError& error)
      {
        throw gcnew TemplateDefinitionError(error);
      }
      catch(const QuickFAST::UsageError& error)
      {
        throw gcnew UsageError(error);
      }

    }



  }
}

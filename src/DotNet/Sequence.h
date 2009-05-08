// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#pragma once

#include <DotNet/UnmanagedPtr.h>
#include <DotNet/StlDotNet.h>
#include <Messages/Sequence.h>

namespace QuickFASTDotNet{
  namespace Messages{

    ref class FieldSet;

    /// Container for a sequence of field groups
    [System::Diagnostics::DebuggerTypeProxy(Sequence::DbView::typeid)]
    public ref class Sequence: public System::Collections::Generic::IEnumerable<FieldSet^>
    {
    public:
      typedef QuickFAST::Messages::Sequence TSequence;
      typedef BoostPtrHolder<QuickFAST::Messages::SequenceCPtr> TSequenceCPtr;

      Sequence();

      /// @brief Allows addressing of a given Field in this FieldSet by index.
      property FieldSet^ default[unsigned int]
      {
        FieldSet^ get(unsigned int index);
      }

      /// @brief Gets and sets the appliction type this FieldSet pertains to.
      property System::String^ ApplicationType
      {
        System::String^ get();
        // Not yet implemented -- only for the encoder
        //void set(System::String^ applicationType);
      }

      ///@brief Gets the number of FieldSet's in this Sequence object.
      property int Count
      {
        int get();
      }

      ///@brief Adds a field set to this sequence.
      // Not yet implemented -- only for the encoder.
      //void Add(FieldSet^ newFieldSet);

      virtual System::Collections::IEnumerator^ GetEnumerator() = System::Collections::IEnumerable::GetEnumerator;

      virtual System::Collections::Generic::IEnumerator<FieldSet^>^ GetSpecializedEnumerator() = System::Collections::Generic::IEnumerable<FieldSet^>::GetEnumerator;

      virtual System::String^ ToString() override
      {
        return System::String::Format("Sequence: FieldSet count: {0}", this->Count);
      }


    internal:

      ref class DbView
      {
      public:
        DbView(Sequence^ viewedContainer)
          :viewedContainer_(viewedContainer)
        {
        }

        [System::Diagnostics::DebuggerBrowsable(System::Diagnostics::DebuggerBrowsableState::RootHidden)]
        property array<FieldSet^>^ Keys
        {
          array<FieldSet^>^ get()
          {
            typedef System::Collections::Generic::List<FieldSet^> TList;
            TList^ outList = gcnew TList();

            System::Collections::Generic::IEnumerator<FieldSet^>^ enumerator = viewedContainer_->GetSpecializedEnumerator();
            do
            {
              outList->Add(enumerator->Current);
            }while(enumerator->MoveNext());

            return outList->ToArray();
          }
        }

      private:
        Sequence^ viewedContainer_;
      };


      ///@brief Constructs a managed Sequence increasing the referenc count of the shared_ptr
      Sequence(const QuickFAST::Messages::SequenceCPtr& sequence);

      ///@brief returns a reference to the unmanaged Sequence object contained in this managed Sequence
      property const TSequence& Ref
      {
        const TSequence& get() { return sequencePtr_.GetRef(); }
      }

      property TSequenceCPtr% SequenceCPtr
      {
        TSequenceCPtr% get() { return sequencePtr_; }
      }

    private:
      ref class SequenceEnumerator:
        public System::Collections::IEnumerator,
        public System::Collections::Generic::IEnumerator<FieldSet^>
      {
      public:
        typedef QuickFAST::Messages::Sequence::const_iterator const_iterator;
        typedef StlDotNet::IteratorHolder<const_iterator> TIteratorHolder;

        // The enumerator requires a reference to the source of the interators
        // to prevent their source from being prematurely released.
        SequenceEnumerator(const_iterator it, const_iterator end, Sequence^ parent)
          : itHolder_(new TIteratorHolder(it, end))
          , parent_(parent)

        {
        }

        ~SequenceEnumerator(){}

        property FieldSet^ GenericCurrent{
          virtual FieldSet^ get () = System::Collections::Generic::IEnumerator<FieldSet^>::Current::get;
        }

        property System::Object^ Current{
          virtual System::Object^ get () = System::Collections::IEnumerator::Current::get;
        }

        virtual bool MoveNext() = IEnumerator::MoveNext;
        virtual void Reset() = IEnumerator::Reset;

      private:
        UnmanagedPtr<StlDotNet::IteratorHolder<const_iterator> > itHolder_;
        Sequence^ parent_;
      };


      TSequenceCPtr sequencePtr_;

    };
  }
}

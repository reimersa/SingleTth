// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME DictObjs

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "Objects/GenInfo.h"
#include "Objects/Particle.h"
#include "Objects/GenParticle.h"
#include "Objects/FlavorParticle.h"
#include "Objects/LorentzVector.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static TClass *GenInfo_Dictionary();
   static void GenInfo_TClassManip(TClass*);
   static void *new_GenInfo(void *p = 0);
   static void *newArray_GenInfo(Long_t size, void *p);
   static void delete_GenInfo(void *p);
   static void deleteArray_GenInfo(void *p);
   static void destruct_GenInfo(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenInfo*)
   {
      ::GenInfo *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::GenInfo));
      static ::ROOT::TGenericClassInfo 
         instance("GenInfo", "GenInfo.h", 4,
                  typeid(::GenInfo), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &GenInfo_Dictionary, isa_proxy, 4,
                  sizeof(::GenInfo) );
      instance.SetNew(&new_GenInfo);
      instance.SetNewArray(&newArray_GenInfo);
      instance.SetDelete(&delete_GenInfo);
      instance.SetDeleteArray(&deleteArray_GenInfo);
      instance.SetDestructor(&destruct_GenInfo);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenInfo*)
   {
      return GenerateInitInstanceLocal((::GenInfo*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::GenInfo*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GenInfo_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::GenInfo*)0x0)->GetClass();
      GenInfo_TClassManip(theClass);
   return theClass;
   }

   static void GenInfo_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR_Dictionary();
   static void ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR_TClassManip(TClass*);
   static void *new_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p = 0);
   static void *newArray_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(Long_t size, void *p);
   static void delete_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p);
   static void deleteArray_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p);
   static void destruct_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >*)
   {
      ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >));
      static ::ROOT::TGenericClassInfo 
         instance("ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >", "Math/GenVector/LorentzVector.h", 48,
                  typeid(::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >) );
      instance.SetNew(&new_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR);
      instance.SetNewArray(&newArray_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR);
      instance.SetDelete(&delete_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR);
      instance.SetDeleteArray(&deleteArray_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR);
      instance.SetDestructor(&destruct_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR);

      ::ROOT::AddClassAlternate("ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >","LorentzVector");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >*)0x0)->GetClass();
      ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *Particle_Dictionary();
   static void Particle_TClassManip(TClass*);
   static void *new_Particle(void *p = 0);
   static void *newArray_Particle(Long_t size, void *p);
   static void delete_Particle(void *p);
   static void deleteArray_Particle(void *p);
   static void destruct_Particle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::Particle*)
   {
      ::Particle *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::Particle));
      static ::ROOT::TGenericClassInfo 
         instance("Particle", "Particle.h", 6,
                  typeid(::Particle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &Particle_Dictionary, isa_proxy, 4,
                  sizeof(::Particle) );
      instance.SetNew(&new_Particle);
      instance.SetNewArray(&newArray_Particle);
      instance.SetDelete(&delete_Particle);
      instance.SetDeleteArray(&deleteArray_Particle);
      instance.SetDestructor(&destruct_Particle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::Particle*)
   {
      return GenerateInitInstanceLocal((::Particle*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::Particle*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *Particle_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::Particle*)0x0)->GetClass();
      Particle_TClassManip(theClass);
   return theClass;
   }

   static void Particle_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *FlavorParticle_Dictionary();
   static void FlavorParticle_TClassManip(TClass*);
   static void *new_FlavorParticle(void *p = 0);
   static void *newArray_FlavorParticle(Long_t size, void *p);
   static void delete_FlavorParticle(void *p);
   static void deleteArray_FlavorParticle(void *p);
   static void destruct_FlavorParticle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::FlavorParticle*)
   {
      ::FlavorParticle *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::FlavorParticle));
      static ::ROOT::TGenericClassInfo 
         instance("FlavorParticle", "FlavorParticle.h", 6,
                  typeid(::FlavorParticle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &FlavorParticle_Dictionary, isa_proxy, 4,
                  sizeof(::FlavorParticle) );
      instance.SetNew(&new_FlavorParticle);
      instance.SetNewArray(&newArray_FlavorParticle);
      instance.SetDelete(&delete_FlavorParticle);
      instance.SetDeleteArray(&deleteArray_FlavorParticle);
      instance.SetDestructor(&destruct_FlavorParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::FlavorParticle*)
   {
      return GenerateInitInstanceLocal((::FlavorParticle*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::FlavorParticle*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *FlavorParticle_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::FlavorParticle*)0x0)->GetClass();
      FlavorParticle_TClassManip(theClass);
   return theClass;
   }

   static void FlavorParticle_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *GenParticle_Dictionary();
   static void GenParticle_TClassManip(TClass*);
   static void *new_GenParticle(void *p = 0);
   static void *newArray_GenParticle(Long_t size, void *p);
   static void delete_GenParticle(void *p);
   static void deleteArray_GenParticle(void *p);
   static void destruct_GenParticle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenParticle*)
   {
      ::GenParticle *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::GenParticle));
      static ::ROOT::TGenericClassInfo 
         instance("GenParticle", "GenParticle.h", 5,
                  typeid(::GenParticle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &GenParticle_Dictionary, isa_proxy, 4,
                  sizeof(::GenParticle) );
      instance.SetNew(&new_GenParticle);
      instance.SetNewArray(&newArray_GenParticle);
      instance.SetDelete(&delete_GenParticle);
      instance.SetDeleteArray(&deleteArray_GenParticle);
      instance.SetDestructor(&destruct_GenParticle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenParticle*)
   {
      return GenerateInitInstanceLocal((::GenParticle*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const ::GenParticle*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *GenParticle_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const ::GenParticle*)0x0)->GetClass();
      GenParticle_TClassManip(theClass);
   return theClass;
   }

   static void GenParticle_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_GenInfo(void *p) {
      return  p ? new(p) ::GenInfo : new ::GenInfo;
   }
   static void *newArray_GenInfo(Long_t nElements, void *p) {
      return p ? new(p) ::GenInfo[nElements] : new ::GenInfo[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenInfo(void *p) {
      delete ((::GenInfo*)p);
   }
   static void deleteArray_GenInfo(void *p) {
      delete [] ((::GenInfo*)p);
   }
   static void destruct_GenInfo(void *p) {
      typedef ::GenInfo current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::GenInfo

namespace ROOT {
   // Wrappers around operator new
   static void *new_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> > : new ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >;
   }
   static void *newArray_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >[nElements] : new ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p) {
      delete ((::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >*)p);
   }
   static void deleteArray_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p) {
      delete [] ((::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >*)p);
   }
   static void destruct_ROOTcLcLMathcLcLLorentzVectorlEROOTcLcLMathcLcLPtEtaPhiE4DlEDouble32_tgRsPgR(void *p) {
      typedef ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >

namespace ROOT {
   // Wrappers around operator new
   static void *new_Particle(void *p) {
      return  p ? new(p) ::Particle : new ::Particle;
   }
   static void *newArray_Particle(Long_t nElements, void *p) {
      return p ? new(p) ::Particle[nElements] : new ::Particle[nElements];
   }
   // Wrapper around operator delete
   static void delete_Particle(void *p) {
      delete ((::Particle*)p);
   }
   static void deleteArray_Particle(void *p) {
      delete [] ((::Particle*)p);
   }
   static void destruct_Particle(void *p) {
      typedef ::Particle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::Particle

namespace ROOT {
   // Wrappers around operator new
   static void *new_FlavorParticle(void *p) {
      return  p ? new(p) ::FlavorParticle : new ::FlavorParticle;
   }
   static void *newArray_FlavorParticle(Long_t nElements, void *p) {
      return p ? new(p) ::FlavorParticle[nElements] : new ::FlavorParticle[nElements];
   }
   // Wrapper around operator delete
   static void delete_FlavorParticle(void *p) {
      delete ((::FlavorParticle*)p);
   }
   static void deleteArray_FlavorParticle(void *p) {
      delete [] ((::FlavorParticle*)p);
   }
   static void destruct_FlavorParticle(void *p) {
      typedef ::FlavorParticle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::FlavorParticle

namespace ROOT {
   // Wrappers around operator new
   static void *new_GenParticle(void *p) {
      return  p ? new(p) ::GenParticle : new ::GenParticle;
   }
   static void *newArray_GenParticle(Long_t nElements, void *p) {
      return p ? new(p) ::GenParticle[nElements] : new ::GenParticle[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenParticle(void *p) {
      delete ((::GenParticle*)p);
   }
   static void deleteArray_GenParticle(void *p) {
      delete [] ((::GenParticle*)p);
   }
   static void destruct_GenParticle(void *p) {
      typedef ::GenParticle current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::GenParticle

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 216,
                  typeid(vector<float>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlEParticlegR_Dictionary();
   static void vectorlEParticlegR_TClassManip(TClass*);
   static void *new_vectorlEParticlegR(void *p = 0);
   static void *newArray_vectorlEParticlegR(Long_t size, void *p);
   static void delete_vectorlEParticlegR(void *p);
   static void deleteArray_vectorlEParticlegR(void *p);
   static void destruct_vectorlEParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<Particle>*)
   {
      vector<Particle> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<Particle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<Particle>", -2, "vector", 216,
                  typeid(vector<Particle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEParticlegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<Particle>) );
      instance.SetNew(&new_vectorlEParticlegR);
      instance.SetNewArray(&newArray_vectorlEParticlegR);
      instance.SetDelete(&delete_vectorlEParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEParticlegR);
      instance.SetDestructor(&destruct_vectorlEParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<Particle> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<Particle>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<Particle>*)0x0)->GetClass();
      vectorlEParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Particle> : new vector<Particle>;
   }
   static void *newArray_vectorlEParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<Particle>[nElements] : new vector<Particle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEParticlegR(void *p) {
      delete ((vector<Particle>*)p);
   }
   static void deleteArray_vectorlEParticlegR(void *p) {
      delete [] ((vector<Particle>*)p);
   }
   static void destruct_vectorlEParticlegR(void *p) {
      typedef vector<Particle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<Particle>

namespace ROOT {
   static TClass *vectorlEGenParticlegR_Dictionary();
   static void vectorlEGenParticlegR_TClassManip(TClass*);
   static void *new_vectorlEGenParticlegR(void *p = 0);
   static void *newArray_vectorlEGenParticlegR(Long_t size, void *p);
   static void delete_vectorlEGenParticlegR(void *p);
   static void deleteArray_vectorlEGenParticlegR(void *p);
   static void destruct_vectorlEGenParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<GenParticle>*)
   {
      vector<GenParticle> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<GenParticle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<GenParticle>", -2, "vector", 216,
                  typeid(vector<GenParticle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEGenParticlegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<GenParticle>) );
      instance.SetNew(&new_vectorlEGenParticlegR);
      instance.SetNewArray(&newArray_vectorlEGenParticlegR);
      instance.SetDelete(&delete_vectorlEGenParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEGenParticlegR);
      instance.SetDestructor(&destruct_vectorlEGenParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<GenParticle> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<GenParticle>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEGenParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<GenParticle>*)0x0)->GetClass();
      vectorlEGenParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEGenParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEGenParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<GenParticle> : new vector<GenParticle>;
   }
   static void *newArray_vectorlEGenParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<GenParticle>[nElements] : new vector<GenParticle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEGenParticlegR(void *p) {
      delete ((vector<GenParticle>*)p);
   }
   static void deleteArray_vectorlEGenParticlegR(void *p) {
      delete [] ((vector<GenParticle>*)p);
   }
   static void destruct_vectorlEGenParticlegR(void *p) {
      typedef vector<GenParticle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<GenParticle>

namespace ROOT {
   static TClass *vectorlEFlavorParticlegR_Dictionary();
   static void vectorlEFlavorParticlegR_TClassManip(TClass*);
   static void *new_vectorlEFlavorParticlegR(void *p = 0);
   static void *newArray_vectorlEFlavorParticlegR(Long_t size, void *p);
   static void delete_vectorlEFlavorParticlegR(void *p);
   static void deleteArray_vectorlEFlavorParticlegR(void *p);
   static void destruct_vectorlEFlavorParticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<FlavorParticle>*)
   {
      vector<FlavorParticle> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<FlavorParticle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<FlavorParticle>", -2, "vector", 216,
                  typeid(vector<FlavorParticle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEFlavorParticlegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<FlavorParticle>) );
      instance.SetNew(&new_vectorlEFlavorParticlegR);
      instance.SetNewArray(&newArray_vectorlEFlavorParticlegR);
      instance.SetDelete(&delete_vectorlEFlavorParticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEFlavorParticlegR);
      instance.SetDestructor(&destruct_vectorlEFlavorParticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<FlavorParticle> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal((const vector<FlavorParticle>*)0x0); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEFlavorParticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<FlavorParticle>*)0x0)->GetClass();
      vectorlEFlavorParticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEFlavorParticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEFlavorParticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FlavorParticle> : new vector<FlavorParticle>;
   }
   static void *newArray_vectorlEFlavorParticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<FlavorParticle>[nElements] : new vector<FlavorParticle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEFlavorParticlegR(void *p) {
      delete ((vector<FlavorParticle>*)p);
   }
   static void deleteArray_vectorlEFlavorParticlegR(void *p) {
      delete [] ((vector<FlavorParticle>*)p);
   }
   static void destruct_vectorlEFlavorParticlegR(void *p) {
      typedef vector<FlavorParticle> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<FlavorParticle>

namespace {
  void TriggerDictionaryInitialization_DictObjs_Impl() {
    static const char* headers[] = {
"Objects/GenInfo.h",
"Objects/Particle.h",
"Objects/GenParticle.h",
"Objects/FlavorParticle.h",
"Objects/LorentzVector.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc7_amd64_gcc700/cms/cmssw/CMSSW_10_2_17/external/slc7_amd64_gcc700/bin/../../../../../../../slc7_amd64_gcc700/lcg/root/6.12.07-gnimlf5/include",
"Objects",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc700/lcg/root/6.12.07-gnimlf5//include/root",
"/cvmfs/cms.cern.ch/slc7_amd64_gcc700/lcg/root/6.12.07-gnimlf5/include",
"/nfs/dust/cms/user/abenecke/CMSSW_10_2_17/CMSSW_10_2_17/src/UHH2/SingleTth/new_signals/NtupleWriter/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "DictObjs dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$Objects/GenParticle.h")))  GenParticle;
namespace std{template <typename _Tp> class __attribute__((annotate("$clingAutoload$bits/allocator.h")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}
class __attribute__((annotate("$clingAutoload$FlavorParticle.h")))  __attribute__((annotate("$clingAutoload$Objects/GenParticle.h")))  FlavorParticle;
class __attribute__((annotate("$clingAutoload$Objects/Particle.h")))  Particle;
class __attribute__((annotate("$clingAutoload$Objects/GenInfo.h")))  GenInfo;
namespace ROOT{namespace Math{template <class ScalarType> class __attribute__((annotate("$clingAutoload$Math/GenVector/PtEtaPhiE4D.h")))  __attribute__((annotate("$clingAutoload$Objects/Particle.h")))  PtEtaPhiE4D;
}}
namespace ROOT{namespace Math{template <class CoordSystem> class __attribute__((annotate("$clingAutoload$Math/GenVector/LorentzVector.h")))  __attribute__((annotate("$clingAutoload$Objects/Particle.h")))  LorentzVector;
}}
typedef double Double32_t __attribute__((annotate("$clingAutoload$RtypesCore.h"))) ;
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> > LorentzVector __attribute__((annotate("$clingAutoload$LorentzVector.h")))  __attribute__((annotate("$clingAutoload$Objects/Particle.h"))) ;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "DictObjs dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "Objects/GenInfo.h"
#include "Objects/Particle.h"
#include "Objects/GenParticle.h"
#include "Objects/FlavorParticle.h"
#include "Objects/LorentzVector.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"FlavorParticle", payloadCode, "@",
"GenInfo", payloadCode, "@",
"GenParticle", payloadCode, "@",
"LorentzVector", payloadCode, "@",
"Particle", payloadCode, "@",
"ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiE4D<Double32_t> >", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("DictObjs",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_DictObjs_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_DictObjs_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_DictObjs() {
  TriggerDictionaryInitialization_DictObjs_Impl();
}

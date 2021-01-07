#ifndef MET_H
#define MET_H

#include "TLorentzVector.h"

/**
 *  @short missing transverse energy container
 *  @author Thomas Peiffer
 */


class MET{
   
 public:
  /// Default constructor
  MET(){
    m_pt=0; 
    m_phi=0; 
    m_mEtSig=0;

  };

  /// Default destructor
  ~MET(){
  };

  /// transverse momentum
  float pt() const{return m_pt;}
  /// phi
  float phi() const{return m_phi;}
  /// transverse momentum significance
  float mEtSig() const{return m_mEtSig;}

  /// set transverse momentum
  void set_pt(float pt){m_pt=pt;}  
  /// set phi
  void set_phi(float phi){m_phi=phi;}
  /// set transverse momentum significance
  void set_mEtSig(float mEtSig){m_mEtSig=mEtSig;}

  /// convert missing transverse energy into 4-vector
  TLorentzVector v4(){
      TLorentzVector met(0,0,0,0);
      met.SetPtEtaPhiM(m_pt, 0, m_phi, 0);
      return met;
  }

 private:
  float m_pt; 
  float m_phi; 
  float m_mEtSig; 

};

#endif

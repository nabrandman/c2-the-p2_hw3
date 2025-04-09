/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#ifndef ATHENAPOOLEXAMPLEALGORITHMS_READSIMPLEELECTRON_H
#define ATHENAPOOLEXAMPLEALGORITHMS_READSIMPLEELECTRON_H

#include "AthenaBaseComps/AthReentrantAlgorithm.h"
#include "AthenaPoolExampleData/ExampleElectronContainer.h"
#include "AthenaPoolExampleData/ExampleHitContainer.h"
#include "AthenaPoolExampleData/ExampleTrackContainer.h"
#include "GaudiKernel/ServiceHandle.h"
#include "StoreGate/ReadDecorHandleKey.h"
#include "StoreGate/ReadHandleKey.h"

class StoreGateSvc;

namespace AthPoolEx {

/** @class ReadExampleElectron
 *  @brief This class provides an example for reading event data objects from
 *Pool.
 **/
class ReadExampleElectron : public AthReentrantAlgorithm {
 public:  // Constructor and Destructor
  /// Standard Service Constructor
  ReadExampleElectron(const std::string& name, ISvcLocator* pSvcLocator);
  /// Destructor
  virtual ~ReadExampleElectron() = default;

 public:
  /// Gaudi Service Interface method implementations:
  virtual StatusCode initialize() override final;
  virtual StatusCode execute(const EventContext& ctx) const override final;
  virtual StatusCode finalize() override final;

 private:
  // Reading through the container of example electrons
  SG::ReadHandleKey<xAOD::ExampleElectronContainer>
      m_exampleElectronContainerKey{this, "ExampleElectronContainerName",
                                    "TestContainer"};

  // Also want to read through '.decor1' decorations, ignoring '.decor2' for
  // sake of demonstration see python/AthenaPoolExample_Write.py:
  //     ItemList = [ ... ,
  //     'xAOD::ExampleElectronAuxContainer#TestContainerAux.-decor2' ]
  SG::ReadDecorHandleKey<xAOD::ExampleElectronContainer> m_decor1Key{
      this, "ExampleElectronContainerDecorKey1", "TestContainer.decor1",
      "decorator1 key"};
  SG::ReadDecorHandleKey<xAOD::ExampleElectronContainer> m_decor_floatKey{
      this, "ExampleElectronContainerDecorKeyFloat", "TestContainer.decor_float",
      "decorator for float key"};

  SG::ReadDecorHandleKey<xAOD::ExampleElectronContainer> m_decor_longdoubleKey{
      this, "ExampleElectronContainerDecorKeyLongDouble", "TestContainer.decor_longdouble",
      "decorator for long double key"};
  SG::ReadDecorHandleKey<xAOD::ExampleElectronContainer> m_decor_boolKey{
      this, "ExampleElectronContainerDecorKeyBool", "TestContainer.decor_bool",
      "decorator for bool key"};
  SG::ReadDecorHandleKey<xAOD::ExampleElectronContainer> m_decor_halfptKey{
      this, "ExampleElectronContainerDecorKeyHalfPt", "TestContainer.decor_halfpt",
      "decorator for half Pt key"};
};

}  // namespace AthPoolEx

#endif

/*
 *  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
 */

#include "WriteExampleElectron.h"

// the user data-class defintions
#include "AthenaPoolExampleData/ExampleElectron.h"
#include "AthenaPoolExampleData/ExampleElectronAuxContainer.h"
#include "AthenaPoolExampleData/ExampleElectronContainer.h"
#include "AthenaPoolExampleData/ExampleHitContainer.h"
#include "AthenaPoolExampleData/ExampleTrackContainer.h"
#include "GaudiKernel/EventContext.h"
#include "StoreGate/WriteDecorHandle.h"
#include "StoreGate/WriteHandle.h"

using namespace AthPoolEx;

//___________________________________________________________________________
WriteExampleElectron::WriteExampleElectron(const std::string& name,
                                           ISvcLocator* pSvcLocator)
    : AthReentrantAlgorithm(name, pSvcLocator) {}

//___________________________________________________________________________
StatusCode WriteExampleElectron::initialize() {
  ATH_MSG_INFO(name() << ": in initialize()");

  ATH_CHECK(m_exampleElectronContainerKey.initialize());
  ATH_CHECK(m_exampleTrackKey.initialize());
  ATH_CHECK(m_exampleHitKey.initialize());
  ATH_CHECK(m_decor1Key.initialize());
  ATH_CHECK(m_decor2Key.initialize());
  ATH_CHECK(m_decor_floatKey.initialize());
  ATH_CHECK(m_decor_longdoubleKey.initialize());
  ATH_CHECK(m_decor_boolKey.initialize());
  ATH_CHECK(m_decor_halfptKey.initialize());

  return StatusCode::SUCCESS;
}

//___________________________________________________________________________
StatusCode WriteExampleElectron::execute(const EventContext& ctx) const {
  ATH_MSG_DEBUG("WriteExampleElectron in execute()");
  std::size_t idx_trk = 0;
  float idx_decor = 0.0f;

  auto elecCont = std::make_unique<xAOD::ExampleElectronContainer>();
  auto elecStore = std::make_unique<xAOD::ExampleElectronAuxContainer>();
  elecCont->setStore(elecStore.get());

  /*
   * Convert ExampleTrack to xAOD::ExampleElectron
   */

  SG::ReadHandle<ExampleTrackContainer> trackCont(m_exampleTrackKey, ctx);

  elecCont->push_back(std::make_unique<xAOD::ExampleElectron>());

  for (const ExampleTrack* track : *trackCont) {
    ATH_MSG_INFO(name() << ": track # " << idx_trk << " pT = " << track->getPT()
                        << ".");

    // proceed to take this tracks pT
    elecCont->back()->setPt(track->getPT());

    // Print out the pT that's being saved
    ATH_MSG_INFO(name() << ": track # " << idx_trk
                        << "is an electron with pT = " << elecCont->back()->pt()
                        << ";");

    idx_trk++;
  }

  SG::WriteHandle<xAOD::ExampleElectronContainer> objs(
      m_exampleElectronContainerKey, ctx);
  ATH_CHECK(objs.record(std::move(elecCont), std::move(elecStore)));

  /*
   * Writing Decorations
   */

  // WriteDecorHandle for the decoration 'TestContainer.decor1'
  SG::WriteDecorHandle<xAOD::ExampleElectronContainer, float> hdl1(m_decor1Key,
                                                                   ctx);

  // And for the second decoration 'TestContainer.decor2'
  SG::WriteDecorHandle<xAOD::ExampleElectronContainer, float> hdl2(m_decor2Key,
                                                                   ctx);
  SG::WriteDecorHandle<xAOD::ExampleElectronContainer, float> hdl3(m_decor_floatKey,
                                                                   ctx);
  SG::WriteDecorHandle<xAOD::ExampleElectronContainer, long double> hdl4(m_decor_longdoubleKey,
									 ctx);
  SG::WriteDecorHandle<xAOD::ExampleElectronContainer, bool> hdl5(m_decor_boolKey,
								  ctx);
  SG::WriteDecorHandle<xAOD::ExampleElectronContainer, double> hdl6(m_decor_halfptKey,
								   ctx);

  if (objs.isValid()) {
    // Access example electrons
    for (const xAOD::ExampleElectron* obj : *objs) {
      hdl1(*obj) = 115.9 + 12.34 * idx_decor;  // decor1
      hdl2(*obj) = 114.9 + idx_decor;          // decor2
      hdl3(*obj) = 33.3 + idx_decor/2.7;
      hdl4(*obj) = 77.5/((long double) idx_decor + 1);
      hdl5(*obj) = (int)idx_decor % 2;
      hdl6(*obj) = (obj->pt())/2;
      

      // Verify writing out two decorations
      ATH_MSG_INFO(name() << ": DecorationWriter: decor1 = " << hdl1(*obj)
		   << ", decor2 = " << hdl2(*obj)
		   << ", decor float = " << hdl3(*obj)
		   << ", decor long double = " << hdl4(*obj)
		   << ", decor bool = " << hdl5(*obj)
		   << ", decor half Pt = " << hdl6(*obj) << ". ");
      idx_decor++;
    }
  } else {
    ATH_MSG_ERROR(name() << ": objs is not valid");
  }

  ATH_MSG_INFO(name() << ": registered all data");
  return StatusCode::SUCCESS;
}

//___________________________________________________________________________
StatusCode WriteExampleElectron::finalize() {
  ATH_MSG_INFO(name() << ": in finalize()");
  return StatusCode::SUCCESS;
}

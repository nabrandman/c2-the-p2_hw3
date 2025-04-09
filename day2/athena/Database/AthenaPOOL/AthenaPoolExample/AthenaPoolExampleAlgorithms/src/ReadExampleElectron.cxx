/*
  Copyright (C) 2002-2024 CERN for the benefit of the ATLAS collaboration
*/

#include "ReadExampleElectron.h"

#include <set>

#include "AthenaPoolExampleData/ExampleElectron.h"
#include "AthenaPoolExampleData/ExampleElectronContainer.h"
#include "EventBookkeeperMetaData/EventBookkeeperCollection.h"
#include "EventInfo/EventID.h"
#include "EventInfo/EventStreamInfo.h"
#include "StoreGate/ReadDecorHandle.h"
#include "StoreGate/ReadHandle.h"

using namespace AthPoolEx;

//___________________________________________________________________________
ReadExampleElectron::ReadExampleElectron(const std::string& name,
                                         ISvcLocator* pSvcLocator)
    : AthReentrantAlgorithm(name, pSvcLocator) {}
//___________________________________________________________________________
StatusCode ReadExampleElectron::initialize() {
  ATH_MSG_INFO(name() << ": in initialize()");

  ATH_CHECK(m_exampleElectronContainerKey.initialize());
  ATH_CHECK(m_decor1Key.initialize());
  ATH_CHECK(m_decor_floatKey.initialize());
  ATH_CHECK(m_decor_longdoubleKey.initialize());
  ATH_CHECK(m_decor_boolKey.initialize());
  ATH_CHECK(m_decor_halfptKey.initialize());

  return StatusCode::SUCCESS;
}
//___________________________________________________________________________
StatusCode ReadExampleElectron::execute(const EventContext& ctx) const {
  ATH_MSG_INFO(name() << ": is executing ...");
  size_t idx = 0;

  // Setup the handle for the ElectronContainer
  SG::ReadHandle<xAOD::ExampleElectronContainer> objs =
      SG::makeHandle(m_exampleElectronContainerKey, ctx);

  // Handle for the decoration
  SG::ReadDecorHandle<xAOD::ExampleElectronContainer, float> hdl1(m_decor1Key,
                                                                  ctx);
  SG::ReadDecorHandle<xAOD::ExampleElectronContainer, float> hdl3(m_decor_floatKey,
                                                                  ctx);
  SG::ReadDecorHandle<xAOD::ExampleElectronContainer, bool> hdl5(m_decor_boolKey,
								 ctx);
  SG::ReadDecorHandle<xAOD::ExampleElectronContainer, double> hdl6(m_decor_halfptKey,
								   ctx);
  
  if (objs.isValid()) {
    // Access the example electrons
    for (const xAOD::ExampleElectron* obj : *objs) {
      ATH_MSG_INFO("ExampleElectron #" << idx << " {pt = " << obj->pt()
                                       << ", charge = " << obj->charge()
                                       << "} ");

      // check to see if the first decor was written and print it out
      if (hdl1.isPresent()) {
        ATH_MSG_INFO("Decoration reader: decor1 = " << hdl1(*obj));
      } else {
        ATH_MSG_INFO("Decoration reader decor1 hasn't been written");
      }
      if (hdl3.isPresent()) {
        ATH_MSG_INFO("Decoration reader: decor float = " << hdl3(*obj));
      } else {
        ATH_MSG_INFO("Decoration reader decor float hasn't been written");
      }
      if (hdl5.isPresent()) {
        ATH_MSG_INFO("Decoration reader: decor bool = " << hdl5(*obj));
      } else {
        ATH_MSG_INFO("Decoration reader decor bool hasn't been written");
      }
      if (hdl6.isPresent()) {
        ATH_MSG_INFO("Decoration reader: decor half Pt = " << hdl6(*obj));
      } else {
        ATH_MSG_INFO("Decoration reader decor half Pt hasn't been written");
      }


      ++idx;  // iterate the index
    }

  } else {
    ATH_MSG_ERROR(" Could not get example electron");
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}
//___________________________________________________________________________
StatusCode ReadExampleElectron::finalize() {
  ATH_MSG_INFO(name() << "in finalize()");
  return StatusCode::SUCCESS;
}

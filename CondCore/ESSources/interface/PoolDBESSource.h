#ifndef CondCore_ESSources_PoolDBESSource_h
#define CondCore_ESSources_PoolDBESSource_h
//
// Package:    CondCore/ESSources
// Class:      PoolDBESSource
//
/**\class PoolDBESSource PoolDBESSource.h CondCore/ESSources/interface/PoolDBESSource.h
 Description: EventSetup source module for serving data from offline database
*/
//
// Author:      Zhen Xie
//
// system include files
#include <string>
#include <map>
#include <set>
// user include files
#include "CondCore/DBCommon/interface/DBSession.h"

#include "FWCore/Framework/interface/DataProxyProvider.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "CondCore/DBCommon/interface/TagMetadata.h"
#include "CondCore/DBCommon/interface/Time.h"
#include <boost/functional/hash.hpp>
namespace edm{
  class ParameterSet;
}
namespace cond{
 class CoralTransaction;
  class Connection;


  struct IOVInfo{
    std::string tag; 
    std::string token;
    std::string label;
    std::string pfn;
    cond::TimeType timetype;
    std::size_t hashvalue()const{
      boost::hash<std::string> hasher;
      std::size_t result=hasher(token+label+pfn);
      return result;
    }
    bool  operator == (const IOVInfo& toCompare ) const {
      if(this->hashvalue()==toCompare.hashvalue()&&this->timetype==toCompare.timetype) return true;
     return false;
    }
    bool operator != (const IOVInfo& toCompare ) const {
      return !(*this==toCompare);
    }
    bool operator<(const IOVInfo& toCompare ) const {
      return this->hashvalue()<toCompare.hashvalue();
    }
  };

}
class PoolDBESSource : public edm::eventsetup::DataProxyProvider,
		       public edm::EventSetupRecordIntervalFinder{
 public:
  PoolDBESSource( const edm::ParameterSet& );
  ~PoolDBESSource();
  
 protected:
  virtual void setIntervalFor(const edm::eventsetup::EventSetupRecordKey&,
			      const edm::IOVSyncValue& , 
			      edm::ValidityInterval&) ;

  virtual void registerProxies(const edm::eventsetup::EventSetupRecordKey& iRecordKey, KeyedProxies& aProxyList) ;

  virtual void newInterval(const edm::eventsetup::EventSetupRecordKey& iRecordType, const edm::ValidityInterval& iInterval) ;    
 private:

  // ----------member data ---------------------------
  cond::DBSession m_session;
 
  typedef boost::shared_ptr<cond::BasePayloadProxy> ProxyP;
  typedef std::map< std::string,  ProxyP> ProxyMap;
  ProxyMap m_proxies;

  typedef std::set< cond::TagMetadata > TagCollection;
  TagCollection m_tagCollection;


 private:

   void fillTagCollectionFromDB( cond::CoralTransaction& coraldb,
				const std::string& roottag,
				std::map<std::string,cond::TagMetadata>& 
				 replacement);
};
#endif

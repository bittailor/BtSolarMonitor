//*************************************************************************************************
//
//  BITTAILOR.CH  -  Bt::Net::Gprs::I_GprsModule
//
//*************************************************************************************************

#ifndef INC__Bt_Net_Gprs_I_GprsModule__hpp
#define INC__Bt_Net_Gprs_I_GprsModule__hpp

namespace Bt {
namespace Net {
namespace Gprs {

class I_GprsModule {
   public:
      class I_Listener {
         virtual ~I_Listener() {}

         virtual void onReady();
      };


      virtual ~I_GprsModule() {}

      virtual void function() = 0;
};

} // namespace Gprs
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Gprs_I_GprsModule__hpp

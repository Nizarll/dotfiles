!function(){"use strict";let e,n;!function(e){e.checkCanOpenEdgeWalletDonation="checkCanOpenEdgeWalletDonation",e.openEdgeWalletDonation="openEdgeWalletDonation"}(e||(e={})),function(e){e.CanOpenWalletDonation="CanOpenWalletDonation"}(n||(n={}));window.edgeWalletDonationRuntime=new class{raiseMessageFromHost(e){const t=e.shift();t&&n.CanOpenWalletDonation}postMessageToHost(e,n){try{"function"==typeof edgeWalletDonationNativeHandler?.sendMessageToHost&&edgeWalletDonationNativeHandler.sendMessageToHost(e,n)}catch(e){}}initialize(){return!0}},window.addEventListener("message",(function(n){if(n?.data?.type==e.openEdgeWalletDonation){const e=[];e.push(n?.data?.id??""),window.edgeWalletDonationRuntime.postMessageToHost(n.data.type,e)}else n?.data?.type==e.checkCanOpenEdgeWalletDonation&&window.edgeWalletDonationRuntime.postMessageToHost(n.data.type,[])}))}();
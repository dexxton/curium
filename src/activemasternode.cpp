// Copyright (c) 2014-2017 The Dash Core developers

// Distributed under the MIT software license, see the accompanying

// file COPYING or http://www.opensource.org/licenses/mit-license.php.



<<<<<<< HEAD
#include "addrman.h"
#include "protocol.h"
=======
>>>>>>> dev-1.12.1.0
#include "activemasternode.h"

#include "masternode.h"
<<<<<<< HEAD
#include "masternodeconfig.h"
#include "spork.h"

//
// Bootup the Masternode, look for a 1000DRK input and register on the network
//
void CActiveMasternode::ManageStatus()
{    
    std::string errorMessage;
=======

#include "masternode-sync.h"

#include "masternodeman.h"

#include "protocol.h"



extern CWallet* pwalletMain;



// Keep track of the active Masternode

CActiveMasternode activeMasternode;



void CActiveMasternode::ManageState()

{
>>>>>>> dev-1.12.1.0

    LogPrint("masternode", "CActiveMasternode::ManageState -- Start\n");

    if(!fMasterNode) {

        LogPrint("masternode", "CActiveMasternode::ManageState -- Not a masternode, returning\n");

<<<<<<< HEAD
    //need correct blocks to send ping
    if(Params().NetworkID() != CBaseChainParams::REGTEST && !masternodeSync.IsBlockchainSynced()) {
        status = ACTIVE_MASTERNODE_SYNC_IN_PROCESS;
        LogPrintf("CActiveMasternode::ManageStatus() - %s\n", GetStatus());
=======
>>>>>>> dev-1.12.1.0
        return;

    }

<<<<<<< HEAD
    if(status == ACTIVE_MASTERNODE_SYNC_IN_PROCESS) status = ACTIVE_MASTERNODE_INITIAL;

    if(status == ACTIVE_MASTERNODE_INITIAL) {
        CMasternode *pmn;
        pmn = mnodeman.Find(pubKeyMasternode);
        if(pmn != NULL) {
            pmn->Check();
            if(pmn->IsEnabled() && pmn->protocolVersion == PROTOCOL_VERSION) EnableHotColdMasterNode(pmn->vin, pmn->addr);
        }
    }

    if(status != ACTIVE_MASTERNODE_STARTED) {

        // Set defaults
        status = ACTIVE_MASTERNODE_NOT_CAPABLE;
        notCapableReason = "";

        if(pwalletMain->IsLocked()){
            notCapableReason = "Wallet is locked.";
            LogPrintf("CActiveMasternode::ManageStatus() - not capable: %s\n", notCapableReason);
            return;
        }

        if(pwalletMain->GetBalance() == 0){
            notCapableReason = "Hot node, waiting for remote activation.";
            LogPrintf("CActiveMasternode::ManageStatus() - not capable: %s\n", notCapableReason);
            return;
        }

        if(strMasterNodeAddr.empty()) {
            if(!GetLocal(service)) {
                notCapableReason = "Can't detect external address. Please use the masternodeaddr configuration option.";
                LogPrintf("CActiveMasternode::ManageStatus() - not capable: %s\n", notCapableReason);
                return;
            }
        } else {
            service = CService(strMasterNodeAddr);
        }

        if(Params().NetworkID() == CBaseChainParams::MAIN) {
            if(service.GetPort() != 9999) {
                notCapableReason = strprintf("Invalid port: %u - only 9999 is supported on mainnet.", service.GetPort());
                LogPrintf("CActiveMasternode::ManageStatus() - not capable: %s\n", notCapableReason);
                return;
            }
        } else if(service.GetPort() == 9999) {
            notCapableReason = strprintf("Invalid port: %u - 9999 is only supported on mainnet.", service.GetPort());
            LogPrintf("CActiveMasternode::ManageStatus() - not capable: %s\n", notCapableReason);
            return;
        }

        LogPrintf("CActiveMasternode::ManageStatus() - Checking inbound connection to '%s'\n", service.ToString());

        CNode *pnode = ConnectNode((CAddress)service, NULL, false);
        if(!pnode){
            notCapableReason = "Could not connect to " + service.ToString();
            LogPrintf("CActiveMasternode::ManageStatus() - not capable: %s\n", notCapableReason);
            return;
        }
        pnode->Release();
=======


    if(Params().NetworkIDString() != CBaseChainParams::REGTEST && !masternodeSync.IsBlockchainSynced()) {

        nState = ACTIVE_MASTERNODE_SYNC_IN_PROCESS;

        LogPrintf("CActiveMasternode::ManageState -- %s: %s\n", GetStateString(), GetStatus());

        return;

    }



    if(nState == ACTIVE_MASTERNODE_SYNC_IN_PROCESS) {

        nState = ACTIVE_MASTERNODE_INITIAL;

    }

>>>>>>> dev-1.12.1.0


    LogPrint("masternode", "CActiveMasternode::ManageState -- status = %s, type = %s, pinger enabled = %d\n", GetStatus(), GetTypeString(), fPingerEnabled);

<<<<<<< HEAD
            if(GetInputAge(vin) < MASTERNODE_MIN_CONFIRMATIONS){
                status = ACTIVE_MASTERNODE_INPUT_TOO_NEW;
                notCapableReason = strprintf("%s - %d confirmations", GetStatus(), GetInputAge(vin));
                LogPrintf("CActiveMasternode::ManageStatus() - %s\n", notCapableReason);
                return;
            }

            LOCK(pwalletMain->cs_wallet);
            pwalletMain->LockCoin(vin.prevout);
=======


    if(eType == MASTERNODE_UNKNOWN) {

        ManageStateInitial();
>>>>>>> dev-1.12.1.0

    }

<<<<<<< HEAD
            if(!darkSendSigner.SetKey(strMasterNodePrivKey, errorMessage, keyMasternode, pubKeyMasternode))
            {
                notCapableReason = "Error upon calling SetKey: " + errorMessage;
                LogPrintf("Register::ManageStatus() - %s\n", notCapableReason);
                return;
            }

            CMasternodeBroadcast mnb;
            if(!CreateBroadcast(vin, service, keyCollateralAddress, pubKeyCollateralAddress, keyMasternode, pubKeyMasternode, errorMessage, mnb)) {
                notCapableReason = "Error on CreateBroadcast: " + errorMessage;
                LogPrintf("Register::ManageStatus() - %s\n", notCapableReason);
                return;
            }

            //send to all peers
            LogPrintf("CActiveMasternode::ManageStatus() - Relay broadcast vin = %s\n", vin.ToString());
            mnb.Relay();

            LogPrintf("CActiveMasternode::ManageStatus() - Is capable master node!\n");
            status = ACTIVE_MASTERNODE_STARTED;

            return;
        } else {
            notCapableReason = "Could not find suitable coins!";
            LogPrintf("CActiveMasternode::ManageStatus() - %s\n", notCapableReason);
            return;
        }
    }

    //send to all peers
    if(!SendMasternodePing(errorMessage)) {
        LogPrintf("CActiveMasternode::ManageStatus() - Error on Ping: %s\n", errorMessage);
    }
}

std::string CActiveMasternode::GetStatus() {
    switch (status) {
    case ACTIVE_MASTERNODE_INITIAL: return "Node just started, not yet activated";
    case ACTIVE_MASTERNODE_SYNC_IN_PROCESS: return "Sync in progress. Must wait until sync is complete to start Masternode";
    case ACTIVE_MASTERNODE_INPUT_TOO_NEW: return strprintf("Masternode input must have at least %d confirmations", MASTERNODE_MIN_CONFIRMATIONS);
    case ACTIVE_MASTERNODE_NOT_CAPABLE: return "Not capable masternode: " + notCapableReason;
    case ACTIVE_MASTERNODE_STARTED: return "Masternode successfully started";
    default: return "unknown";
=======


    if(eType == MASTERNODE_REMOTE) {

        ManageStateRemote();

    } else if(eType == MASTERNODE_LOCAL) {

        // Try Remote Start first so the started local masternode can be restarted without recreate masternode broadcast.

        ManageStateRemote();

        if(nState != ACTIVE_MASTERNODE_STARTED)

            ManageStateLocal();

    }



    SendMasternodePing();

}



std::string CActiveMasternode::GetStateString() const

{

    switch (nState) {

        case ACTIVE_MASTERNODE_INITIAL:         return "INITIAL";

        case ACTIVE_MASTERNODE_SYNC_IN_PROCESS: return "SYNC_IN_PROCESS";

        case ACTIVE_MASTERNODE_INPUT_TOO_NEW:   return "INPUT_TOO_NEW";

        case ACTIVE_MASTERNODE_NOT_CAPABLE:     return "NOT_CAPABLE";

        case ACTIVE_MASTERNODE_STARTED:         return "STARTED";

        default:                                return "UNKNOWN";

    }

}



std::string CActiveMasternode::GetStatus() const

{

    switch (nState) {

        case ACTIVE_MASTERNODE_INITIAL:         return "Node just started, not yet activated";

        case ACTIVE_MASTERNODE_SYNC_IN_PROCESS: return "Sync in progress. Must wait until sync is complete to start Masternode";

        case ACTIVE_MASTERNODE_INPUT_TOO_NEW:   return strprintf("Masternode input must have at least %d confirmations", Params().GetConsensus().nMasternodeMinimumConfirmations);

        case ACTIVE_MASTERNODE_NOT_CAPABLE:     return "Not capable masternode: " + strNotCapableReason;

        case ACTIVE_MASTERNODE_STARTED:         return "Masternode successfully started";

        default:                                return "Unknown";

>>>>>>> dev-1.12.1.0
    }

}

<<<<<<< HEAD
bool CActiveMasternode::SendMasternodePing(std::string& errorMessage) {
    if(status != ACTIVE_MASTERNODE_STARTED) {
        errorMessage = "Masternode is not in a running status";
=======


std::string CActiveMasternode::GetTypeString() const

{

    std::string strType;

    switch(eType) {

    case MASTERNODE_UNKNOWN:

        strType = "UNKNOWN";

        break;

    case MASTERNODE_REMOTE:

        strType = "REMOTE";

        break;

    case MASTERNODE_LOCAL:

        strType = "LOCAL";

        break;

    default:

        strType = "UNKNOWN";

        break;

    }

    return strType;

}



bool CActiveMasternode::SendMasternodePing()

{

    if(!fPingerEnabled) {

        LogPrint("masternode", "CActiveMasternode::SendMasternodePing -- %s: masternode ping service is disabled, skipping...\n", GetStateString());

>>>>>>> dev-1.12.1.0
        return false;

    }


<<<<<<< HEAD
    if(!darkSendSigner.SetKey(strMasterNodePrivKey, errorMessage, keyMasternode, pubKeyMasternode))
    {
        errorMessage = strprintf("Error upon calling SetKey: %s\n", errorMessage);
=======

    if(!mnodeman.Has(vin)) {

        strNotCapableReason = "Masternode not in masternode list";

        nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

        LogPrintf("CActiveMasternode::SendMasternodePing -- %s: %s\n", GetStateString(), strNotCapableReason);

>>>>>>> dev-1.12.1.0
        return false;

    }

<<<<<<< HEAD
    LogPrintf("CActiveMasternode::SendMasternodePing() - Relay Masternode Ping vin = %s\n", vin.ToString());
    
    CMasternodePing mnp(vin);
    if(!mnp.Sign(keyMasternode, pubKeyMasternode))
    {
        errorMessage = "Couldn't sign Masternode Ping";
=======


    CMasternodePing mnp(vin);

    if(!mnp.Sign(keyMasternode, pubKeyMasternode)) {

        LogPrintf("CActiveMasternode::SendMasternodePing -- ERROR: Couldn't sign Masternode Ping\n");

>>>>>>> dev-1.12.1.0
        return false;

<<<<<<< HEAD
    // Update lastPing for our masternode in Masternode list
    CMasternode* pmn = mnodeman.Find(vin);
    if(pmn != NULL)
    {
        if(pmn->IsPingedWithin(MASTERNODE_PING_SECONDS, mnp.sigTime)){
            errorMessage = "Too early to send Masternode Ping";
            return false;
        }

        pmn->lastPing = mnp;
        mnodeman.mapSeenMasternodePing.insert(make_pair(mnp.GetHash(), mnp));

        //mnodeman.mapSeenMasternodeBroadcast.lastPing is probably outdated, so we'll update it
        CMasternodeBroadcast mnb(*pmn);
        uint256 hash = mnb.GetHash();
        if(mnodeman.mapSeenMasternodeBroadcast.count(hash)) mnodeman.mapSeenMasternodeBroadcast[hash].lastPing = mnp;

        mnp.Relay();

        return true;
    }
    else
    {
        // Seems like we are trying to send a ping while the Masternode is not registered in the network
        errorMessage = "Darksend Masternode List doesn't include our Masternode, shutting down Masternode pinging service! " + vin.ToString();
        status = ACTIVE_MASTERNODE_NOT_CAPABLE;
        notCapableReason = errorMessage;
=======
    }



    // Update lastPing for our masternode in Masternode list

    if(mnodeman.IsMasternodePingedWithin(vin, MASTERNODE_MIN_MNP_SECONDS, mnp.sigTime)) {

        LogPrintf("CActiveMasternode::SendMasternodePing -- Too early to send Masternode Ping\n");

>>>>>>> dev-1.12.1.0
        return false;

    }

<<<<<<< HEAD
}

bool CActiveMasternode::CreateBroadcast(std::string strService, std::string strKeyMasternode, std::string strTxHash, std::string strOutputIndex, std::string& errorMessage, CMasternodeBroadcast &mnb, bool fOffline) {
    CTxIn vin;
    CPubKey pubKeyCollateralAddress;
    CKey keyCollateralAddress;
    CPubKey pubKeyMasternode;
    CKey keyMasternode;

    //need correct blocks to send ping
    if(!fOffline && !masternodeSync.IsBlockchainSynced()) {
        errorMessage = "Sync in progress. Must wait until sync is complete to start Masternode";
        LogPrintf("CActiveMasternode::CreateBroadcast() - %s\n", errorMessage);
        return false;
    }

    if(!darkSendSigner.SetKey(strKeyMasternode, errorMessage, keyMasternode, pubKeyMasternode))
    {
        errorMessage = strprintf("Can't find keys for masternode %s - %s", strService, errorMessage);
        LogPrintf("CActiveMasternode::CreateBroadcast() - %s\n", errorMessage);
        return false;
    }

    if(!GetMasterNodeVin(vin, pubKeyCollateralAddress, keyCollateralAddress, strTxHash, strOutputIndex)) {
        errorMessage = strprintf("Could not allocate vin %s:%s for masternode %s", strTxHash, strOutputIndex, strService);
        LogPrintf("CActiveMasternode::CreateBroadcast() - %s\n", errorMessage);
        return false;
    }

    CService service = CService(strService);
    if(Params().NetworkID() == CBaseChainParams::MAIN) {
        if(service.GetPort() != 9999) {
            errorMessage = strprintf("Invalid port %u for masternode %s - only 9999 is supported on mainnet.", service.GetPort(), strService);
            LogPrintf("CActiveMasternode::CreateBroadcast() - %s\n", errorMessage);
            return false;
        }
    } else if(service.GetPort() == 9999) {
        errorMessage = strprintf("Invalid port %u for masternode %s - 9999 is only supported on mainnet.", service.GetPort(), strService);
        LogPrintf("CActiveMasternode::CreateBroadcast() - %s\n", errorMessage);
        return false;
    }

    addrman.Add(CAddress(service), CNetAddr("127.0.0.1"), 2*60*60);

    return CreateBroadcast(vin, CService(strService), keyCollateralAddress, pubKeyCollateralAddress, keyMasternode, pubKeyMasternode, errorMessage, mnb);
}

bool CActiveMasternode::CreateBroadcast(CTxIn vin, CService service, CKey keyCollateralAddress, CPubKey pubKeyCollateralAddress, CKey keyMasternode, CPubKey pubKeyMasternode, std::string &errorMessage, CMasternodeBroadcast &mnb) {
    // wait for reindex and/or import to finish
    if (fImporting || fReindex) return false;

    CMasternodePing mnp(vin);
    if(!mnp.Sign(keyMasternode, pubKeyMasternode)){
        errorMessage = strprintf("Failed to sign ping, vin: %s", vin.ToString());
        LogPrintf("CActiveMasternode::CreateBroadcast() -  %s\n", errorMessage);
        mnb = CMasternodeBroadcast();
        return false;
    }

    mnb = CMasternodeBroadcast(service, vin, pubKeyCollateralAddress, pubKeyMasternode, PROTOCOL_VERSION);
    mnb.lastPing = mnp;
    if(!mnb.Sign(keyCollateralAddress)){
        errorMessage = strprintf("Failed to sign broadcast, vin: %s", vin.ToString());
        LogPrintf("CActiveMasternode::CreateBroadcast() - %s\n", errorMessage);
        mnb = CMasternodeBroadcast();
        return false;
=======


    mnodeman.SetMasternodeLastPing(vin, mnp);



    LogPrintf("CActiveMasternode::SendMasternodePing -- Relaying ping, collateral=%s\n", vin.ToString());

    mnp.Relay();



    return true;

}



void CActiveMasternode::ManageStateInitial()

{

    LogPrint("masternode", "CActiveMasternode::ManageStateInitial -- status = %s, type = %s, pinger enabled = %d\n", GetStatus(), GetTypeString(), fPingerEnabled);



    // Check that our local network configuration is correct

    if (!fListen) {

        // listen option is probably overwritten by smth else, no good

        nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

        strNotCapableReason = "Masternode must accept connections from outside. Make sure listen configuration option is not overwritten by some another parameter.";

        LogPrintf("CActiveMasternode::ManageStateInitial -- %s: %s\n", GetStateString(), strNotCapableReason);

        return;

    }



    bool fFoundLocal = false;

    {

        LOCK(cs_vNodes);



        // First try to find whatever local address is specified by externalip option

        fFoundLocal = GetLocal(service) && CMasternode::IsValidNetAddr(service);

        if(!fFoundLocal) {

            // nothing and no live connections, can't do anything for now

            if (vNodes.empty()) {

                nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

                strNotCapableReason = "Can't detect valid external address. Will retry when there are some connections available.";

                LogPrintf("CActiveMasternode::ManageStateInitial -- %s: %s\n", GetStateString(), strNotCapableReason);

                return;

            }

            // We have some peers, let's try to find our local address from one of them

            BOOST_FOREACH(CNode* pnode, vNodes) {

                if (pnode->fSuccessfullyConnected && pnode->addr.IsIPv4()) {

                    fFoundLocal = GetLocal(service, &pnode->addr) && CMasternode::IsValidNetAddr(service);

                    if(fFoundLocal) break;

                }

            }

        }

    }



    if(!fFoundLocal) {

        nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

        strNotCapableReason = "Can't detect valid external address. Please consider using the externalip configuration option if problem persists. Make sure to use IPv4 address only.";

        LogPrintf("CActiveMasternode::ManageStateInitial -- %s: %s\n", GetStateString(), strNotCapableReason);

        return;

    }



    int mainnetDefaultPort = Params(CBaseChainParams::MAIN).GetDefaultPort();

    if(Params().NetworkIDString() == CBaseChainParams::MAIN) {

        if(service.GetPort() != mainnetDefaultPort) {

            nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

            strNotCapableReason = strprintf("Invalid port: %u - only %d is supported on mainnet.", service.GetPort(), mainnetDefaultPort);

            LogPrintf("CActiveMasternode::ManageStateInitial -- %s: %s\n", GetStateString(), strNotCapableReason);

            return;

        }

    } else if(service.GetPort() == mainnetDefaultPort) {

        nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

        strNotCapableReason = strprintf("Invalid port: %u - %d is only supported on mainnet.", service.GetPort(), mainnetDefaultPort);

        LogPrintf("CActiveMasternode::ManageStateInitial -- %s: %s\n", GetStateString(), strNotCapableReason);

        return;

    }



    LogPrintf("CActiveMasternode::ManageStateInitial -- Checking inbound connection to '%s'\n", service.ToString());



    if(!ConnectNode((CAddress)service, NULL, true)) {

        nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

        strNotCapableReason = "Could not connect to " + service.ToString();

        LogPrintf("CActiveMasternode::ManageStateInitial -- %s: %s\n", GetStateString(), strNotCapableReason);

        return;

>>>>>>> dev-1.12.1.0
    }



<<<<<<< HEAD
bool CActiveMasternode::GetMasterNodeVin(CTxIn& vin, CPubKey& pubkey, CKey& secretKey, std::string strTxHash, std::string strOutputIndex) {
    // wait for reindex and/or import to finish
    if (fImporting || fReindex) return false;

    // Find possible candidates
    TRY_LOCK(pwalletMain->cs_wallet, fWallet);
    if(!fWallet) return false;

    vector<COutput> possibleCoins = SelectCoinsMasternode();
    COutput *selectedOutput;

    // Find the vin
    if(!strTxHash.empty()) {
        // Let's find it
        uint256 txHash(strTxHash);
        int outputIndex = atoi(strOutputIndex.c_str());
        bool found = false;
        BOOST_FOREACH(COutput& out, possibleCoins) {
            if(out.tx->GetHash() == txHash && out.i == outputIndex)
            {
                selectedOutput = &out;
                found = true;
                break;
            }
        }
        if(!found) {
            LogPrintf("CActiveMasternode::GetMasterNodeVin - Could not locate valid vin\n");
            return false;
        }
    } else {
        // No output specified,  Select the first one
        if(possibleCoins.size() > 0) {
            selectedOutput = &possibleCoins[0];
        } else {
            LogPrintf("CActiveMasternode::GetMasterNodeVin - Could not locate specified vin from possible list\n");
            return false;
        }
=======
    // Default to REMOTE

    eType = MASTERNODE_REMOTE;



    // Check if wallet funds are available

    if(!pwalletMain) {

        LogPrintf("CActiveMasternode::ManageStateInitial -- %s: Wallet not available\n", GetStateString());

        return;

>>>>>>> dev-1.12.1.0
    }



<<<<<<< HEAD
// Extract Masternode vin information from output
bool CActiveMasternode::GetVinFromOutput(COutput out, CTxIn& vin, CPubKey& pubkey, CKey& secretKey) {
    // wait for reindex and/or import to finish
    if (fImporting || fReindex) return false;
=======
    if(pwalletMain->IsLocked()) {
>>>>>>> dev-1.12.1.0

        LogPrintf("CActiveMasternode::ManageStateInitial -- %s: Wallet is locked\n", GetStateString());

        return;

    }



    if(pwalletMain->GetBalance() < 10000*COIN) {

        LogPrintf("CActiveMasternode::ManageStateInitial -- %s: Wallet balance is < 10000 CURIUM\n", GetStateString());

        return;

    }



    // Choose coins to use

    CPubKey pubKeyCollateral;

    CKey keyCollateral;



    // If collateral is found switch to LOCAL mode

    if(pwalletMain->GetMasternodeVinAndKeys(vin, pubKeyCollateral, keyCollateral)) {

        eType = MASTERNODE_LOCAL;

    }



    LogPrint("masternode", "CActiveMasternode::ManageStateInitial -- End status = %s, type = %s, pinger enabled = %d\n", GetStatus(), GetTypeString(), fPingerEnabled);

}



void CActiveMasternode::ManageStateRemote()

{
<<<<<<< HEAD
    vector<COutput> vCoins;
    vector<COutput> filteredCoins;
    vector<COutPoint> confLockedCoins;

    // Temporary unlock MN coins from masternode.conf
    if(GetBoolArg("-mnconflock", true)) {
        uint256 mnTxHash;
        BOOST_FOREACH(CMasternodeConfig::CMasternodeEntry mne, masternodeConfig.getEntries()) {
            mnTxHash.SetHex(mne.getTxHash());
            COutPoint outpoint = COutPoint(mnTxHash, atoi(mne.getOutputIndex().c_str()));
            confLockedCoins.push_back(outpoint);
            pwalletMain->UnlockCoin(outpoint);
        }
    }
=======
>>>>>>> dev-1.12.1.0

    LogPrint("masternode", "CActiveMasternode::ManageStateRemote -- Start status = %s, type = %s, pinger enabled = %d, pubKeyMasternode.GetID() = %s\n", 

             GetStatus(), fPingerEnabled, GetTypeString(), pubKeyMasternode.GetID().ToString());



    mnodeman.CheckMasternode(pubKeyMasternode);

    masternode_info_t infoMn = mnodeman.GetMasternodeInfo(pubKeyMasternode);

    if(infoMn.fInfoValid) {

        if(infoMn.nProtocolVersion != PROTOCOL_VERSION) {

            nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

            strNotCapableReason = "Invalid protocol version";

            LogPrintf("CActiveMasternode::ManageStateRemote -- %s: %s\n", GetStateString(), strNotCapableReason);

            return;

        }

        if(service != infoMn.addr) {

            nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

            strNotCapableReason = "Specified IP doesn't match our external address.";

            LogPrintf("CActiveMasternode::ManageStateRemote -- %s: %s\n", GetStateString(), strNotCapableReason);

            return;

        }

        if(!CMasternode::IsValidStateForAutoStart(infoMn.nActiveState)) {

            nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

            strNotCapableReason = strprintf("Masternode in %s state", CMasternode::StateToString(infoMn.nActiveState));

            LogPrintf("CActiveMasternode::ManageStateRemote -- %s: %s\n", GetStateString(), strNotCapableReason);

            return;

        }

        if(nState != ACTIVE_MASTERNODE_STARTED) {

            LogPrintf("CActiveMasternode::ManageStateRemote -- STARTED!\n");

            vin = infoMn.vin;

            service = infoMn.addr;

            fPingerEnabled = true;

            nState = ACTIVE_MASTERNODE_STARTED;

<<<<<<< HEAD
    // Lock MN coins from masternode.conf back if they where temporary unlocked
    if(!confLockedCoins.empty()) {
        BOOST_FOREACH(COutPoint outpoint, confLockedCoins)
            pwalletMain->LockCoin(outpoint);
    }

    // Filter
    BOOST_FOREACH(const COutput& out, vCoins)
    {
        if(out.tx->vout[out.i].nValue == 1000*COIN) { //exactly
            filteredCoins.push_back(out);
=======
>>>>>>> dev-1.12.1.0
        }

    }

    else {

        nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

        strNotCapableReason = "Masternode not in masternode list";

        LogPrintf("CActiveMasternode::ManageStateRemote -- %s: %s\n", GetStateString(), strNotCapableReason);

    }

}



void CActiveMasternode::ManageStateLocal()

{

<<<<<<< HEAD
    status = ACTIVE_MASTERNODE_STARTED;

    //The values below are needed for signing mnping messages going forward
    vin = newVin;
    service = newService;
=======
    LogPrint("masternode", "CActiveMasternode::ManageStateLocal -- status = %s, type = %s, pinger enabled = %d\n", GetStatus(), GetTypeString(), fPingerEnabled);

    if(nState == ACTIVE_MASTERNODE_STARTED) {
>>>>>>> dev-1.12.1.0

        return;

    }



    // Choose coins to use

    CPubKey pubKeyCollateral;

    CKey keyCollateral;



    if(pwalletMain->GetMasternodeVinAndKeys(vin, pubKeyCollateral, keyCollateral)) {

        int nInputAge = GetInputAge(vin);

        if(nInputAge < Params().GetConsensus().nMasternodeMinimumConfirmations){

            nState = ACTIVE_MASTERNODE_INPUT_TOO_NEW;

            strNotCapableReason = strprintf(_("%s - %d confirmations"), GetStatus(), nInputAge);

            LogPrintf("CActiveMasternode::ManageStateLocal -- %s: %s\n", GetStateString(), strNotCapableReason);

            return;

        }



        {

            LOCK(pwalletMain->cs_wallet);

            pwalletMain->LockCoin(vin.prevout);

        }



        CMasternodeBroadcast mnb;

        std::string strError;

        if(!CMasternodeBroadcast::Create(vin, service, keyCollateral, pubKeyCollateral, keyMasternode, pubKeyMasternode, strError, mnb)) {

            nState = ACTIVE_MASTERNODE_NOT_CAPABLE;

            strNotCapableReason = "Error creating mastenode broadcast: " + strError;

            LogPrintf("CActiveMasternode::ManageStateLocal -- %s: %s\n", GetStateString(), strNotCapableReason);

            return;

        }



        fPingerEnabled = true;

        nState = ACTIVE_MASTERNODE_STARTED;



        //update to masternode list

        LogPrintf("CActiveMasternode::ManageStateLocal -- Update Masternode List\n");

        mnodeman.UpdateMasternodeList(mnb);

        mnodeman.NotifyMasternodeUpdates();



        //send to all peers

        LogPrintf("CActiveMasternode::ManageStateLocal -- Relay broadcast, vin=%s\n", vin.ToString());

        mnb.Relay();

    }

}

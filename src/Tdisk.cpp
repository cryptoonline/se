//
//  Tdisk.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "Tdisk.h"

Tdisk::Tdisk(){
//    T = new TBlock*[MAX_T_SIZE];
    for(int i = 0; i < MAX_T_SIZE; i++)
        T[i] = new TBlock();
}

void Tdisk::addFile(fileID fid, PRSubset &prSubset){
    uint32_t TRecordIndex = fid.getPRPofHigherID();
    struct CRI cri;
    memcpy(cri.fid, fid.get(), 32);
    cri.prSubsetSeed = prSubset.getSeed();
    cri.prSubsetSize = prSubset.getSize();
    map[TRecordIndex].push_back(cri);
}

void Tdisk::finalize(Ddisk &D){
    for(__gnu_cxx::hash_map<uint32_t , std::vector<CRI> >::iterator iter=map.begin(); iter != map.end(); ++iter){
        vector<CRI> cri = iter->second;
        unsigned char* TRecordIndex = new unsigned char[4];
        memcpy(TRecordIndex, cri[0].fid, 4);
        fileID fid(TRecordIndex, true);
        
        uint32_t prSubsetSize = (uint32_t)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE) * (uint32_t)BLOW_UP;
        PRSubset prSubset(prSubsetSize);
        
        D.addBlocks(static_cast<unsigned char*>(static_cast<void*>(cri.data())), cri.size()*sizeof(CRI), fid, prSubset);
        T[*(uint32_t*)TRecordIndex]->set(prSubset.getSeed(), prSubset.getSize(), *(uint32_t*)TRecordIndex);
    }
}

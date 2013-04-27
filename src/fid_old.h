//
//  fid.h
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/10/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#ifndef BlindStorage_fid_h
#define BlindStorage_fid_h

using namespace std;

class fileID{
private:
    uint32_t higherFid;
    uint32_t lowerFid; /*! First bit of this is used as CRI indicator */
    uint64_t Fid;
    void CRIFileFlag(){
        lowerFid |= 0x10000000;
    }
    
    
public:
    
    fileID(string filename){
        
    }
    
    /*! Free blocks have 0 file id */
    fileID(uint32_t higherFid = 0, uint32_t lowerFid = 0){
        Fid = 0;
        this->higherFid = higherFid;
        this->lowerFid = lowerFid;
    }
    
    void set(uint64_t Fid){
        this->Fid = Fid;
    }
    
    uint64_t get(){
        return Fid;
    }

    uint64_t make(bool CRIFileFlag = false){
        if(CRIFileFlag)
            this->CRIFileFlag();
        Fid |= higherFid;
        Fid << 32;
        Fid |= lowerFid;
        return Fid;
    }

    uint32_t getHigherFid(){
        return higherFid;
    }
    
    uint32_t getLowerFid(){
        return lowerFid;
    }
};



#endif

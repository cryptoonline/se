//
//  Tdisk.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 4/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include "Tdisk.h"

Tdisk::Tdisk(){
    T = new TBlock*[MAX_T_SIZE];
    for(int i = 0; i < MAX_T_SIZE; i++){
        cout << (double)i*100/(double)MAX_T_SIZE << "\% T blocks initialized";
		cout.flush();
		cout << '\r';
		T[i] = new TBlock();
	}
	cout << endl;
}

Tdisk::~Tdisk(){
}

void Tdisk::addFile(string filename, PRSubset &prSubset){
	fileID fid(filename);
	cout << "Filename is " << filename << endl;
    uint32_t TRecordIndex = fid.getPRPofHigherID();
    cout << "Index in " << __FUNCTION__ << " is " << TRecordIndex << endl;
	struct CRI cri;
    memcpy(cri.fid, fid.get(), 32);
	unsigned char* FID = fid.get();
	//for( int i = 0; i < 32; i++)
	//	printf("%02X ", FID[i]);
	//cout << endl;
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
		
		fileID fidcompare("/home/naveed2/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/1.");

			cout << "************************************Unencrypted****************************************************************" << endl; 
			cout << prSubset.getSeed() << endl;
			cout << prSubset.getSize() << endl;
			cout << *(uint32_t*)TRecordIndex <<endl;
			unsigned char* blockencrypted = T[*(uint32_t*)TRecordIndex]->getEncrypted();
			unsigned char* blockdecrypted = T[*(uint32_t*)TRecordIndex]->getDecrypted();
			for(int i = 0; i < 12; i++)
				printf("%02X ", blockencrypted[i]);
			cout << endl;
			for(int i = 0; i < 12; i++)
				printf("%02X ", blockdecrypted[i]);
			cout << endl;
			cout << "******************************************************************************************************" << endl; 
		}			 
	for(int i = 0; i < MAX_T_SIZE; i++)
		T[i]->encryptIfEmpty();
}

void Tdisk::writeToDisk(){
	ofstream file("data/T", std::ios_base::binary);
	
	for(int i = 0; i < MAX_T_SIZE; i++){
		cout << i*100/MAX_T_SIZE << "\% blocks processed";
		cout.flush();
		cout << "\r";
		file.write(reinterpret_cast<char*>(T[i]->getEncrypted()), T_BLOCK_SIZE);
	}
	cout << endl;
}

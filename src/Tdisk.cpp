//
//  Tdisk.cpp
//  BlindStorage
//

#include "Tdisk.h"

Tdisk::Tdisk(){
	T = new TBlock*[TTOTAL_BLOCKS];
	for(int i = 0; i < TTOTAL_BLOCKS; i++)
		T[i] = new TBlock(i); 
	Key key("TdiskKey", AES_KEY_SIZE);
	byte keyBytes[AES_KEY_SIZE];
	key.get(keyBytes);
	T[0]->setKey(keyBytes);
}

Tdisk::~Tdisk(){
	delete[] T;
}

void Tdisk::addFile(fileID fid, PRSubset prSubset){
  higherfid_t higherfid = fid.getHigherID();
	byte lowerID[LOWERFID_SIZE];
	fid.getLowerID(lowerID);
	if(criMap.count(higherfid) == 0){
		CRI cri(higherfid);
		criMap.insert(std::pair<higherfid_t, CRI>(higherfid, cri));
	}
	criMap[higherfid].addFile(prSubset.getSize(), prSubset.getSeed(), lowerID);
}

void Tdisk::finalize(Ddisk &D){

	for(unordered_map<higherfid_t , CRI>::iterator iter=criMap.begin(); iter != criMap.end(); ++iter){
		higherfid_t higherfid = iter->first;
		CRI cri = iter->second;
		criBytes.resize((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE);
		//cri.makeBytes(criBytes);
		//byte* criBytes = new byte[(int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE]();
	//	shared_ptr<byte> criBytes(new byte[(int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE]());
		cri.makeBytes(criBytes);
		byte fidBytes[FILEID_SIZE] = {0};
		memcpy(fidBytes, static_cast<byte*>(static_cast<void*>(&higherfid)), HIGHERFID_SIZE);
		fileID fid(fidBytes);

		PRSubset prSubset((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);

		T[higherfid]->make(prSubset.getSize(), prSubset.getSeed());

		byte fidBytesRet[FILEID_SIZE];
		fid.get(fidBytesRet);
		D.addFile(criBytes.data(), cri.size(), fid, prSubset, true);
		}			 

	for(int i = 0; i < TTOTAL_BLOCKS; i++)
		T[i]->encryptIfEmpty();
}

void Tdisk::getBlock(t_index_t index, TBlock& block){
	block = *T[index];
}

// void Tdisk::writeToDisk(){
// 	ofstream file("data/T", std::ios_base::binary);
// 	
// 	for(int i = 0; i < MAX_T_SIZE; i++){
// 		cout << i*100/MAX_T_SIZE << "\% blocks processed";
// 		cout.flush();
// 		cout << "\r";
// 		file.write(reinterpret_cast<char*>(T[i]->getEncrypted()), T_BLOCK_SIZE);
// 	}
// 	cout << endl;
// }

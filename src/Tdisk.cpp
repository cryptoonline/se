//
//  Tdisk.cpp
//  BlindStorage
//

#include "Tdisk.h"

Tdisk::Tdisk(){
	T = new TBlock*[TTOTAL_BLOCKS];
	for(int i = 0; i < TTOTAL_BLOCKS; i++)
		T[i] = new TBlock(i); 
	Key key(T_KEYFILE, AES_KEY_SIZE);
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
		criMap.insert(std::make_pair<higherfid_t, CRI>(higherfid, cri));
	}
	criMap[higherfid].addFile(prSubset.getSize(), prSubset.getSeed(), lowerID);
}

void Tdisk::finalize(Ddisk &D){
//	cout << __PRETTY_FUNCTION__ << criMap.size() << "Begin: " << criMap.begin() << " End: " << criMap.end() << endl;
	int i = 0;
	for(unordered_map<higherfid_t , CRI>::iterator iter=criMap.begin(); iter != criMap.end(); ++iter){
		higherfid_t higherfid = iter->first;
		CRI cri = iter->second;
//		criBytes.reserve((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE);
//		cri.makeBytes(criBytes);
		//byte* criBytes = new byte[(int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE]();
	//	shared_ptr<byte> criBytes(new byte[(int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOCK_SIZE]());
		vector<byte> emptyVector;
		criBytes.push_back(emptyVector);
		cri.makeBytes(criBytes[i], __PRETTY_FUNCTION__);
//		printhex(criBytes[i], "CRI BYTES");
		byte fidBytes[FILEID_SIZE] = {0};
		memcpy(fidBytes, static_cast<byte*>(static_cast<void*>(&higherfid)), HIGHERFID_SIZE);
		fileID fid(fidBytes);

		b_index_t numBlocks = ((int)ceil((double)cri.size()/(double)MAX_BLOCK_DATA_SIZE)*BLOW_UP);
		PRSubset prSubset(numBlocks);
		b_index_t blockIndices[prSubset.getSize()];
		prSubset.get(blockIndices, prSubset.getSize());
		printdec(blockIndices, numBlocks, "Indices");
		
		T[higherfid]->make(prSubset.getSize(), prSubset.getSeed());

		byte tblock[TBLOCK_SIZE];
		T[higherfid]->getDecrypted(tblock);
		cout << "Higher FID" << higherfid << endl;
		printhex(tblock, TBLOCK_SIZE, "TBLOCK");
//		printhex(criBytes, __PRETTY_FUNCTION__);
		byte fidBytesRet[FILEID_SIZE];
		fid.get(fidBytesRet);
		cout << "CRI Bytes size is " << cri.size() << endl;
		D.addFile(criBytes[i].data(), cri.size(), fid, prSubset, true);
		i++;
		}			 

	for(int i = 0; i < TTOTAL_BLOCKS; i++)
		T[i]->encryptIfEmpty();
}

void Tdisk::getBlock(t_index_t index, TBlock& block){
	block = *T[index];
}

void Tdisk::writeToDisk(){
	ofstream file(T_FILE, std::ios_base::binary);
 	
	for(int i = 0; i < TTOTAL_BLOCKS; i++){
 		cout << (i+1)*100/TTOTAL_BLOCKS << "\% blocks processed.";
		cout.flush();
		cout << "\r";
		byte blockEncrypted[TBLOCK_SIZE];
		T[i]->getEncrypted(blockEncrypted);
		file.write(reinterpret_cast<char*>(blockEncrypted), TBLOCK_SIZE);
	}
	cout << endl;
}

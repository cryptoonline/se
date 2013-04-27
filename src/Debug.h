////
////  Debug.h
////  BlindStorage
////
////  Created by Muhammad Naveed on 4/13/13.
////  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
////
//
//#ifndef BlindStorage_Debug_h
//#define BlindStorage_Debug_h
//
//#include<stdint.h>
//void print(string tag, int integer){
//    unsigned char bytes[sizeof integer];
//    std::copy(static_cast<const unsigned char*>(static_cast<const void*>(&integer)),
//              static_cast<const unsigned char*>(static_cast<const void*>(&integer)) + sizeof integer,
//              bytes);
//    
//    cout << endl << "*********************" << tag << " (start)*********************" << endl;
//    for( int i = 0; i < sizeof integer; i++)
//        cout << "[" << i << "] --> " << (uint8_t)bytes[i] << '\t';
//    cout << endl << "**********************" << tag << " (end)**********************" << endl;
//}
//
//void print(string tag, unsigned char* value, uint32_t size){
//    cout << endl << "*********************" << tag << " (start)*********************" << endl;
//    for( int i = 0; i < size; i++)
//        cout << value[i];
//    cout << endl << "**********************" << tag << " (end)**********************" << endl;
//}
//
//void print(string tag, char* value, uint32_t size){
//    cout << endl << "*********************" << tag << " (start)*********************" << endl;
//    for( int i = 0; i < size; i++)
//        cout << i << " --> " << (int)value[i] << '\t';
//    cout << endl << "**********************" << tag << " (end)**********************" << endl;
//}
//
//
//
//
//#endif

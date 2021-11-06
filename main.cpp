/*
Cache Simulator
Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
s = log2(#sets)   b = log2(block size)  t=32-s-b
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>
#include <stdio.h>
#include <map>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss




struct config{
    unsigned long L1blocksize;
    unsigned long L1setsize;
    unsigned long L1size;
    
    unsigned long L2blocksize;
    unsigned long L2setsize;
    unsigned long L2size;
    /*-------------------------------added-------------------------------*/
    unsigned long L3blocksize;
    unsigned long L3setsize;
    unsigned long L3size;
    /*-------------------------------/added-------------------------------*/
};

/* you can define the cache class here, or design your own data structure for L1 and L2 cache*/

class cache {
    config config_params;
    unsigned long setIndexBit_L1, tag_L1, blockOffset_L1, setIndexBit_L2, tag_L2, blockOffset_L2;
    /*-------------------------------added-------------------------------*/
    unsigned long setIndexBit_L3, tag_L3, blockOffset_L3;
    /*-------------------------------/added-------------------------------*/
    vector<vector<unsigned long> > cache_L1;
    vector<vector<unsigned long> > cache_L2;
    vector<vector<unsigned long> > validBit_L1, validBit_L2;
    /*-------------------------------added-------------------------------*/
    vector<vector<unsigned long> > cache_L3, validBit_L3;
    /*-------------------------------/added-------------------------------*/


    public :
        cache(config config_param){
            this->config_params = config_param;

            this->blockOffset_L1 = (unsigned long)log2(config_params.L1blocksize);
            this->blockOffset_L2 = (unsigned long)log2(config_params.L2blocksize);
            /*-------------------------------added-------------------------------*/
            this->blockOffset_L3 = (unsigned long)log2(config_params.L3blocksize);
            /*-------------------------------/added-------------------------------*/


            this->setIndexBit_L1 = (unsigned long)log2((config_params.L1size)*(pow(2,10))/(config_params.L1blocksize*config_params.L1setsize));
            this->setIndexBit_L2 = (unsigned long)log2((config_params.L2size)*(pow(2,10))/(config_params.L2blocksize*config_params.L2setsize));
            /*-------------------------------added-------------------------------*/
            this->setIndexBit_L3 = (unsigned long)log2((config_params.L3size)*(pow(2,10))/(config_params.L3blocksize*config_params.L3setsize));
            /*-------------------------------/added-------------------------------*/

            this->tag_L1 = 32 - setIndexBit_L1 - blockOffset_L1;
            this->tag_L2 = 32 - setIndexBit_L2 - blockOffset_L2;
            /*-------------------------------added-------------------------------*/
            this->tag_L3 = 32 - setIndexBit_L3 - blockOffset_L3;
            /*-------------------------------/added-------------------------------*/

            unsigned long indexRow_L1 = (unsigned long)pow(2, setIndexBit_L1);
            unsigned long indexRow_L2 = (unsigned long)pow(2, setIndexBit_L2);
            /*-------------------------------added-------------------------------*/
            unsigned long indexRow_L3 = (unsigned long)pow(2, setIndexBit_L3);
            /*-------------------------------/added-------------------------------*/

            this->validBit_L1.resize(this->config_params.L1setsize);
            for(int i=0;i<this->config_params.L1setsize;i++){
                this->validBit_L1[i].resize(indexRow_L1);
            }

            this->validBit_L2.resize(this->config_params.L2setsize);
            for(int i=0;i<this->config_params.L2setsize;i++){
                this->validBit_L2[i].resize(indexRow_L2);
            }
            /*-------------------------------added-------------------------------*/
            this->validBit_L3.resize(this->config_params.L3setsize);
            for(int i=0;i<this->config_params.L3setsize;i++){
                this->validBit_L3[i].resize(indexRow_L3);
            }
            /*-------------------------------/added-------------------------------*/



            unsigned long blockSize_L1 = (unsigned long)pow(2, blockOffset_L1);
            unsigned long blockSize_L2 = (unsigned long)pow(2, blockOffset_L2);
            /*-------------------------------added-------------------------------*/
            unsigned long blockSize_L3 = (unsigned long)pow(2, blockOffset_L3);
            /*-------------------------------/added-------------------------------*/
            
            this->cache_L1 = createCache(config_params.L1setsize, indexRow_L1, blockSize_L1);
            this->cache_L2 = createCache(config_params.L2setsize, indexRow_L2, blockSize_L2);
            /*-------------------------------added-------------------------------*/
            this->cache_L3 = createCache(config_params.L3setsize, indexRow_L3, blockSize_L3);
            /*-------------------------------/added-------------------------------*/
        }

        

        vector<string> getBits(bitset<32> addr) {
            string stringAddr = addr.to_string();
            vector<string> bits(9);
            bits[0] = stringAddr.substr(0, this->tag_L1);
            bits[1] = stringAddr.substr(this->tag_L1, this->setIndexBit_L1);
            bits[2] = stringAddr.substr(this->tag_L1+this->setIndexBit_L1, this->blockOffset_L1);

            bits[3] = stringAddr.substr(0, this->tag_L2);
            bits[4] = stringAddr.substr(this->tag_L2, this->setIndexBit_L2);
            bits[5] = stringAddr.substr(this->tag_L2+this->setIndexBit_L2,this->blockOffset_L2);
            /*-------------------------------added-------------------------------*/
            bits[6] = stringAddr.substr(0, this->tag_L3);
            bits[7] = stringAddr.substr(this->tag_L3, this->setIndexBit_L3);
            bits[8] = stringAddr.substr(this->tag_L3+this->setIndexBit_L3,this->blockOffset_L3);
            /*-------------------------------/added-------------------------------*/
            return bits;
        }

        vector<vector<unsigned long> > createCache(unsigned long &way, unsigned long &indexRow, unsigned long &blockSize){
            vector<vector<unsigned long> > myCache;

            myCache.resize((unsigned long)way);
            for(int i=0; i<way;i++) {
                myCache[i].resize(indexRow);

            }

            return myCache;
        }

    vector<vector<unsigned long> > getL1Cache(){
        return this->cache_L1;
    }

    vector<vector<unsigned long> > getL2Cache(){
        return this->cache_L2;
    }
    /*-------------------------------added-------------------------------*/
    vector<vector<unsigned long> > getL3Cache(){
        return this->cache_L3;
    }
    /*-------------------------------/added-------------------------------*/
    vector<vector<unsigned long> > getValidL1(){
        return this->validBit_L1;
    }

    vector<vector<unsigned long> > getValidL2(){
        return this->validBit_L2;
    }
    /*-------------------------------added-------------------------------*/
    vector<vector<unsigned long> > getValidL3(){
        return this->validBit_L3;
    }
    /*-------------------------------/added-------------------------------*/
};

map<unsigned long, bool> isWaysFull(vector<vector<unsigned long> > &L_cache, unsigned long way_L, long index_L){

    map<unsigned long, bool> cacheMap;

    for(unsigned long i=0;i<way_L;i++){

        cacheMap[i] = L_cache[i][index_L] != 0;
    }
    
    return cacheMap;
}

void loadTag(vector<vector<unsigned long> > &L_cache, vector<vector<unsigned long> > &L_validBit, unsigned long &way_L,unsigned long &wayNumber, long &index_L, long tag_L){

    L_cache[wayNumber][index_L] = (unsigned long)tag_L;
    L_validBit[wayNumber][index_L] = 1;

}

unsigned long incCounter (unsigned long &way_L, unsigned long &wayNumber){

return (wayNumber+1)%way_L;

}

int main(int argc, char* argv[]){



    config cacheconfig;
    ifstream cache_params;
    string dummyLine;

    cache_params.open("cacheconfig.txt");

        while (!cache_params.eof())  // read config file
        {
            cache_params >> dummyLine;
            cache_params >> cacheconfig.L1blocksize;
            cache_params >> cacheconfig.L1setsize;
            cache_params >> cacheconfig.L1size;
            cache_params >> dummyLine;
            cache_params >> cacheconfig.L2blocksize;
            cache_params >> cacheconfig.L2setsize;
            cache_params >> cacheconfig.L2size;
            cache_params >> dummyLine;
            /*-------------------------------added-------------------------------*/
            cache_params >> cacheconfig.L3blocksize;
            cache_params >> cacheconfig.L3setsize;
            cache_params >> cacheconfig.L3size;
            /*-------------------------------/added-------------------------------*/
        }

    
    if(cacheconfig.L1setsize == 0){
        cacheconfig.L1setsize = cacheconfig.L1size*1024/cacheconfig.L1blocksize;
    }
    if(cacheconfig.L2setsize == 0){
        cacheconfig.L2setsize = cacheconfig.L2size*1024/cacheconfig.L2blocksize;
    }
    /*-------------------------------added-------------------------------*/
    if(cacheconfig.L3setsize == 0){
        cacheconfig.L3setsize = cacheconfig.L3size*1024/cacheconfig.L3blocksize;
    }
    /*-------------------------------/added-------------------------------*/
    
    cache cacheObj(cacheconfig);



        // Implement by you:
        // initialize the hierarchy cache system with those configs
        // probably you may define a Cache class for L1 and L2, or any data structure you like
    vector<vector<unsigned long> > L1_cache = cacheObj.getL1Cache();
    vector<vector<unsigned long> > L2_cache = cacheObj.getL2Cache();
    
    vector<vector<unsigned long> > validBit_L1 = cacheObj.getValidL1();
    vector<vector<unsigned long> > validBit_L2 = cacheObj.getValidL2();
    /*-------------------------------added-------------------------------*/
    vector<vector<unsigned long> > L3_cache = cacheObj.getL3Cache();
    vector<vector<unsigned long> > validBit_L3 = cacheObj.getValidL3();
    /*-------------------------------/added-------------------------------*/
    
    unsigned long sizeL1Way = L1_cache.size();
    unsigned long sizeL2Way = L2_cache.size();
    unsigned long row_L1 = L1_cache[0].size();
    unsigned long row_L2 = L2_cache[0].size();
    /*-------------------------------added-------------------------------*/
    unsigned long sizeL3Way = L3_cache.size();
    unsigned long row_L3 = L3_cache[0].size();
    /*-------------------------------/added-------------------------------*/

    


    int L1AcceState = 0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
    int L2AcceState = 0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;
    /*-------------------------------added-------------------------------*/
    int L3AcceState = 0;
    /*-------------------------------/added-------------------------------*/

        ifstream traces;
        ofstream tracesout;
        string outname;
        outname=string("traceout.txt");

        traces.open("trace.txt");
        tracesout.open(outname.c_str());

        string line;
        string accesstype;  // the Read/Write access type from the memory trace;
        string xaddr;       // the address from the memory trace store in hex;
        unsigned int addr;  // the address from the memory trace store in unsigned int;
        bitset<32> accessaddr; // the address from the memory trace store in the bitset;

        vector<unsigned long> counter_L1, counter_L2, counter_L3;
        unsigned long temp;
        counter_L1.resize(row_L1);
        counter_L2.resize(row_L2);
    /*-------------------------------added-------------------------------*/
        counter_L3.resize(row_L3);
    /*-------------------------------/added-------------------------------*/
        int count=0;
    /*-------------------------------added-------------------------------*/
        int totalRead = 0;
        int totalWrite = 0;
        int totalReadMiss = 0;
        int totalWriteMiss = 0;
    /*-------------------------------/added-------------------------------*/

        if (traces.is_open() && tracesout.is_open()) {
            while (getline(traces, line)) {   // read mem access file and access Cache
                count++;

                istringstream iss(line);
                if (!(iss >> accesstype >> xaddr)) { break; }

                stringstream saddr(xaddr);
                saddr >> std::hex >> addr;

                accessaddr = bitset<32>(addr);


                vector<string> bits = cacheObj.getBits(accessaddr);
                string strTagL1 = bits[0];
                string strIndexL1 = bits[1];
                string strOffsetL1 = bits[2];
                string strTagL2 = bits[3];
                string strIndexL2 = bits[4];
                string strOffsetL2 = bits[5];
                /*-------------------------------added-------------------------------*/
                string strTagL3 = bits[6];
                string strIndexL3 = bits[7];
                string strOffsetL3 = bits[8];
                /*-------------------------------/added-------------------------------*/

                long  index_L1, index_L2, index_L3;
                char * ptr;
                long  tag_L1= strtol(strTagL1.c_str(), & ptr, 2);
                long  tag_L2= strtol(strTagL2.c_str(), & ptr, 2);
                /*-------------------------------added-------------------------------*/
                long  tag_L3= strtol(strTagL3.c_str(), & ptr, 2);
                /*-------------------------------/added-------------------------------*/

                if(strIndexL1 != ""){
                    index_L1= strtol(strIndexL1.c_str(), & ptr, 2);
                }
                else{
                    index_L1 = 0;
                }

                if(strIndexL2 != ""){
                    index_L2= strtol(strIndexL2.c_str(), & ptr, 2);
                }
                else{
                    index_L2 = 0;
                }
                /*-------------------------------added-------------------------------*/
                if(strIndexL3 != "") {
                    index_L3= strtol(strIndexL3.c_str(), & ptr, 2);
                }
                else {
                    index_L3 = 0;
                }
                /*-------------------------------/added-------------------------------*/

                unsigned long hit_L1=0, hit_L2=0, hit_L3=0;

               

                //tag bit matching on L1
                for(unsigned long i=0; i<sizeL1Way; i++){
                    //way checking
                    if(L1_cache[i][index_L1] == tag_L1 && validBit_L1[i][index_L1]==1){
                        // hit

                        hit_L1=1;
                        break;
                    }
                    else{
                        //miss
                        hit_L1=0;
                    }
                }

                if(hit_L1== 0){
                    // goto L2

                    for(unsigned long i=0; i<sizeL2Way; i++){
                        //way checking
                        if(L2_cache[i][index_L2] == tag_L2){
                            // hit

                            hit_L2=1;
                            break;
                        }
                        else{
                            //miss

                            hit_L2=0;
                        }
                    }

                }
                
                /*-------------------------------added-------------------------------*/
                if(hit_L2 == 0){    // L3로 이동
                    for(unsigned long i=0; i<sizeL3Way; i++) {
                        if(L3_cache[i][index_L3] == tag_L3){
                            hit_L3=1;
                            break;
                        }
                        else{
                            hit_L3=0;
                        }
                    }
                }
                /*-------------------------------/added-------------------------------*/

                map<unsigned long, bool> isFull;

                // access the L1 and L2 Cache according to the trace;
                if (accesstype.compare("R") == 0) {
                    totalRead++;
                    //Implement by you:
                    // read access to the L1 Cache,
                    //  and then L2 (if required),
                    //  update the L1 and L2 access state variable;

                    // L3까지 모두 miss일 때
                    if(hit_L1==0 && hit_L2 ==0 && hit_L3==0){
                        totalReadMiss++;
                        L1AcceState = RM;
                        L2AcceState = RM;
                        L3AcceState = RM;

                        //Goto main memory and fetch the data of that tag
                        //update the L3 cache
                        //checking which L3 Ways are empty
                        isFull = isWaysFull(L3_cache, sizeL3Way, index_L3);
                        temp = 0;
                        // 카운터에 의해 나온 way number가 비었는지 아닌지 체크
                        while(isFull[counter_L3[index_L3]] && temp<sizeL3Way){
                            counter_L3[index_L3] = (++counter_L3[index_L3])%sizeL3Way;
                            ++temp;
                        }
                        // L3 캐시에 태그 로드
                        loadTag(L3_cache, validBit_L3, sizeL3Way,counter_L3[index_L3], index_L3, tag_L3);
                        counter_L3[index_L3] = incCounter(sizeL3Way, counter_L3[index_L3]);


                        // update the L2 cache
                        //checking which L2 Ways are empty
                        isFull = isWaysFull(L2_cache, sizeL2Way, index_L2);
                        temp = 0;

                        //counter_L2 will be having the Way number in which we have to load the data
                        //checking whether Way number provided by counter is empty or not
                        while(isFull[counter_L2[index_L2]] && temp<sizeL2Way){
                            counter_L2[index_L2] = (++counter_L2[index_L2])%sizeL2Way;
                            ++temp;
                        }

                        // Finally loading Tag onto L2 cache
                        loadTag(L2_cache, validBit_L2, sizeL2Way,counter_L2[index_L2], index_L2, tag_L2);
                        counter_L2[index_L2] = incCounter(sizeL2Way, counter_L2[index_L2]);


                        // update the L1 cache
                        isFull = isWaysFull(L1_cache, sizeL1Way, index_L1);
                        temp = 0;

                        while(isFull[counter_L1[index_L1]] && temp<sizeL1Way){
                            counter_L1[index_L1] = (++counter_L1[index_L1])%sizeL1Way;
                            ++temp;
                        }

                        loadTag(L1_cache, validBit_L1, sizeL1Way, counter_L1[index_L1], index_L1, tag_L1);
                        counter_L1[index_L1] = incCounter(sizeL1Way, counter_L1[index_L1]);
                    }
                    // L3에서 히트부분 추가
                    else if(hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 1){
                        L1AcceState = RM;
                        L2AcceState = RM;
                        L3AcceState = RH;
                        
                        loadTag(L2_cache, validBit_L2, sizeL1Way, counter_L2[index_L2], index_L2, tag_L2);
                        counter_L2[index_L2] = incCounter(sizeL2Way, counter_L2[index_L1]);
                        
                        loadTag(L1_cache, validBit_L1, sizeL1Way, counter_L1[index_L1], index_L1, tag_L1);
                        counter_L1[index_L1] = incCounter(sizeL1Way, counter_L1[index_L1]);
                    }
                    
                    else if(hit_L1 == 0 && hit_L2 == 1){

                        L1AcceState = RM;
                        L2AcceState = RH;
                        L3AcceState = NA;

                        //when there is a read miss in L1 and read hit in L2, the L1 cache might have to evict some data to make room for the data returned by the L2 cache.
                        //If the evicted data is dirty, this will result in a write access to L2. If the write access to L2 results in a hit no data will be evicted/replaced.
                        //If it results in a miss, the data will be forwarded to main memory without changing the L2 cache state since the L2 cache is a write-no-allocate cache

                        //put content of l2 into l1
                        loadTag(L1_cache, validBit_L1, sizeL1Way, counter_L1[index_L1], index_L1, tag_L1);
                        counter_L1[index_L1] = incCounter(sizeL1Way, counter_L1[index_L1]);


                    }
                    else if(hit_L1 == 1){

                        L1AcceState = RH;
                        L2AcceState = NA;
                        L3AcceState = NA;

                    }
                    
                    

                    isFull = isWaysFull(L1_cache, sizeL1Way, index_L1);

                   

                } else {
                    totalWrite++;
                    //Implement by you:
                    // write access to the L1 Cache,
                    //and then L2 (if required),
                    //update the L1 and L2 access state variable;

                    if (hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 0) {
                        totalWriteMiss++;
                        L1AcceState = WM;
                        L2AcceState = WM;
                        L3AcceState = WM;
                        //

                    } else if (hit_L1 == 0 && hit_L2 == 0 && hit_L3 == 1) { // L3에서 히트했을 때 추가
                        L1AcceState = WM;
                        L2AcceState = WM;
                        L3AcceState = WH;
                    } else if (hit_L1 == 0 && hit_L2 == 1) {
                        L1AcceState = WM;
                        L2AcceState = WH;
                        L3AcceState = NA;

                        //Do not allocate any space on L1. Pass the write access to L2
                        //Goto L2 cache and update the data array with the new data on that tag


                    } else if (hit_L1 == 1) {
                        L1AcceState = WH;
                        L2AcceState = NA;
                        L3AcceState = NA;
                        // update the data array on this tag at L1


                    }

                    
                }

                /*-------------------------------added-------------------------------*/
                tracesout << L1AcceState << " " << L2AcceState << " " << L3AcceState
                          << endl;  // Output hit/miss results for L1 and L2 to the output file;
                /*-------------------------------/added-------------------------------*/
                
                
            }
            traces.close();
            tracesout.close();
        } else cout << "\n Unable to open trace or traceout file ";

    cout << "Total Read: " << totalRead << endl;
    cout << "Total Read Misses: " << totalReadMiss << endl;
    cout << "Read Hit Rate " << 1 - ((float)totalReadMiss/(float)totalRead) << endl;
    cout << "Total Write: " << totalWrite << endl;
    cout << "Total Write Misses: " << totalWriteMiss << endl;
    cout << "Write Hit Rate: " << 1 - ((float)totalWriteMiss/(float)totalWrite) << endl;
    


    return 0;
}

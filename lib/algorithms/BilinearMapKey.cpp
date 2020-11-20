/*
 * Duy Nguyen - duy@cs.brown.edu
 *         May 18, 2011
 */

#include <algorithms/BilinearMapKey.hpp>

#include <bilinear/G1_DCLXVI.hpp>
#include <bilinear/G2_DCLXVI.hpp>

#include <bilinear/Scalar_DCLXVI.hpp>
#include <utils/Pointers.hpp>

BilinearMapKey::BilinearMapKey(){
    _sk = unique_new<ScalarDCLXVI>();
    _pk = unique_new<BilinearMapKey::PublicKey>();
}

BilinearMapKey::~BilinearMapKey(){
//	if(_sk){
//		delete _sk;
//		_sk = NULL;
//	}
//	if(_pk){
//		for(size_t k=0; k<_pk->size(); k++){
//			for(size_t i=0; i<((*_pk)[k])->size(); i++){
//				if((*(*_pk)[k])[i]){
//					delete (*(*_pk)[k])[i];
//					(*(*_pk)[k])[i] = NULL;
//				}
//			}
//			delete (*_pk)[k];
//			(*_pk)[k] = NULL;
//		}
//		delete _pk;
//		_pk = NULL;
//	}
}

Scalar& BilinearMapKey::getSecretKey() const{
    return *(_sk);
}

BilinearMapKey::PublicKey& BilinearMapKey::getPublicKey() const{
    return *(_pk);
}

void BilinearMapKey::readSkFromFile(const char *fName){
	std::ifstream in(fName, std::ios::in | std::ios::binary);
	_sk->readFromFile(in);
	in.close();
}

void BilinearMapKey::writeSkToFile(const char *fName) const{
	std::ofstream out(fName, std::ios::out | std::ios::binary);
	_sk->writeToFile(out);
	out.close();
}

void BilinearMapKey::readPkFromFile(const char *fName){
	std::ifstream in(fName, std::ios::in | std::ios::binary);

	size_t pkSize;
	in.read((char *)&pkSize, sizeof(pkSize));

    for(size_t i=0; i<pkSize; i++){
        std::unique_ptr<G> g1 = unique_new<G1DCLXVI>();
		g1->readFromFile(in);
        _pk->first.push_back(std::move(g1));
	}
	for(size_t i=0; i<pkSize; i++){
        std::unique_ptr<G> g2 = unique_new<G2DCLXVI>();
		g2->readFromFile(in);
        _pk->second.push_back(std::move(g2));
	}

	in.close();

	std::cout<<"Loading public key done."<<" Size = "<<pkSize<<" element(s)."<<std::endl;
}

void BilinearMapKey::writePkToFile(const char *fName) const{
	std::ofstream out(fName, std::ios::out | std::ios::binary);

    size_t pkSize = _pk->first.size();
	out.write((char *)&pkSize, sizeof(pkSize));


    for(size_t i=0; i<_pk->first.size(); i++){
        _pk->first.at(i)->writeToFile(out);
    }
    for(size_t i=0; i<_pk->second.size(); i++){
        _pk->second.at(i)->writeToFile(out);
    }


	out.close();
}


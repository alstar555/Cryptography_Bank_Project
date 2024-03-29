#ifndef CRYPTOGRAPHY_BANK_PROJECT_DH_H
#define CRYPTOGRAPHY_BANK_PROJECT_DH_H

#include <boost/multiprecision/cpp_int.hpp>

class DH {
private:
    boost::multiprecision::cpp_int local_secret;
    boost::multiprecision::cpp_int p;
    boost::multiprecision::cpp_int g;
    boost::multiprecision::cpp_int shared_secret;

    void initialize_p();
public:
    DH();

    // compute g^local_secret mod p
    boost::multiprecision::cpp_int get_send_value();

    void build_shared_secret(boost::multiprecision::cpp_int from_val);

    boost::multiprecision::cpp_int get_shared_secret() const { return shared_secret; }
};


#endif //CRYPTOGRAPHY_BANK_PROJECT_DH_H

/*

 PHI
 2025/12/21

 Phi C++ Project
 include/encryption/gzip_basic.hpp

 Zevi Berlin

*/

#ifndef GZIP_BASIC_HPP
#define GZIP_BASIC_HPP

#include <string>
#include <stdexcept>

#include <zlib.h>

//---------> [ Config. Separator ] <---------\\ 

namespace phi::encryption {

std::string gzipCompress(const std::string& input);

//================={ Header Item Separator }=================\\ 

std::string gzipDecompress(const std::string& input);

}  // namespace phi::encryption

#endif /* GZIP_BASIC_HPP */
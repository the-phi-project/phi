/*

 PHI
 2025/12/07

 Phi C++ Project
 include/database/structs.hpp

 Zevi Berlin

*/

#ifndef DATABASE_STRUCTS_HPP
#define DATABASE_STRUCTS_HPP

#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"

#include "utils/str_utils.hpp"

using json = nlohmann::json;

//---------> [ Config. Separator ] <---------\\ 

namespace phi::database {

struct self_t {
    std::string rsa2048_pub;
    std::string rsa2048_priv;
    std::string rsa4096_pub;
    std::string rsa4096_priv;
    std::string kyber512_pub;
    std::string kyber512_priv;
    std::string kyber768_pub;
    std::string kyber768_priv;

    std::unordered_map<std::string, std::string*> MAP;

    self_t() {
      this->MAP["rsa2048_pub"] = &this->rsa2048_pub;
      this->MAP["rsa2048_priv"] = &this->rsa2048_priv;
      this->MAP["rsa4096_pub"] = &this->rsa4096_pub;
      this->MAP["rsa4096_priv"] = &this->rsa4096_priv;
      this->MAP["kyber512_pub"] = &this->kyber512_pub;
      this->MAP["kyber512_priv"] = &this->kyber512_priv;
      this->MAP["kyber768_pub"] = &this->kyber768_pub;
      this->MAP["kyber768_priv"] = &this->kyber768_priv;
    }
};

//================={ Header Item Separator }=================\\ 

struct contact_t {
    int id;
    std::string name;
    std::string rsa2048;
    std::string rsa4096;
    std::string kyber512;
    std::string kyber768;

    std::unordered_map<std::string, std::string*> MAP;

    contact_t() {
      this->MAP["name"] = &this->name;
      this->MAP["rsa2048"] = &this->rsa2048;
      this->MAP["rsa4096"] = &this->rsa4096;
      this->MAP["kyber512"] = &this->kyber512;
      this->MAP["kyber768"] = &this->kyber768;
    }

    //=====[ Declaration Separator ]=====\\ 

    /*std::unordered_map<std::string, std::string> getUpdatedFields(self_t& updated) {
      std::unordered_map<std::string, std::string> fields;

      for (const auto& [field, ptr] : this->MAP) {
        if (*ptr != *updated.MAP[field]) {
          fields[field] = *updated.MAP[field];
        }
      }

      return fields;
    }*/
};

//================={ Header Item Separator }=================\\ 

struct message_t {
    int symmetric_key_len;
    std::string content;
    std::string hash;
    std::string nonce;  // empty if not using ChaCha20-Poly1305
    std::string encrypted_key;
    std::string iv;  // empty if not using AES
    int symmetric;
    int asymmetric;

    //=====[ Declaration Separator ]=====\\ 

    std::string to_json_str() const {
      json j;

      j["symmetric_key_len"] = this->symmetric_key_len;
      j["content"] = toB64(this->content);
      j["hash"] = toB64(this->hash);
      j["nonce"] = toB64(this->nonce);
      j["encrypted_key"] = toB64(this->encrypted_key);
      j["iv"] = toB64(this->iv);
      j["symmetric"] = this->symmetric;
      j["asymmetric"] = this->asymmetric;

      return j.dump();
    }

    bool from_json_str(const std::string& json_str) {
      if (!json::accept(json_str)) return false;

      json j = json::parse(json_str);

      this->symmetric_key_len = j["symmetric_key_len"].get<int>();
      this->content = fromB64(j["content"].get<std::string>());
      this->hash = fromB64(j["hash"].get<std::string>());
      this->nonce = fromB64(j["nonce"].get<std::string>());
      this->encrypted_key = fromB64(j["encrypted_key"].get<std::string>());
      this->iv = fromB64(j["iv"].get<std::string>());
      this->symmetric = j["symmetric"].get<int>();
      this->asymmetric = j["asymmetric"].get<int>();

      return true;
    }
};

}  // namespace phi::database

#endif /* DATABASE_STRUCTS_HPP */
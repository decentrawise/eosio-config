// configuration for EOSIO smart contracts
#pragma once

#include <cmath>
#include <string>

#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>


struct [[eosio::table("config"), eosio::contract("profile")]] config_data {
  double dval1 = 0.90;
  double dval2 = 1.0;
  uint32_t uival1 = 5;
  uint32_t uival2 = 0;

  void set(eosio::name key, std::string value) {
    switch(key) {
      case eosio::name("dval1"):        this->dval1             = to_double(value); break;
      case eosio::name("dval2"):        this->dval2             = to_double(value); break;
      case eosio::name("uival1"):       this->uival1            = to_ui32(value); break;
      case eosio::name("uival2"):       this->uival2            = to_ui32(value); break;
      default:
        eosio::check(false, "invalid config key");
    }
  }

private:

  inline uint32_t to_ui32(std::string s) {
    // return std::atoi(s.c_str());
    return std::stoul(s);
  }

  inline double to_double(std::string s) {
    // return std::stod(s);   // std::stod doesn't work, as people say online, so implement our own
    std::size_t i = s.find(".");
    int digits = s.length() - i - 1;
    s.erase(i, 1); 
    return atoi(s.c_str()) / pow(10, digits);
  }  

};

typedef eosio::singleton<eosio::name("config"), config_data> config_singleton;

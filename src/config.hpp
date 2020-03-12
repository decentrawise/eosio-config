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
  eosio::name master = eosio::name("defaultacc");

  void set(eosio::name key, std::string value) {
    switch(key) {
      case eosio::name("dval1"):        this->dval1             = to_decimal<double>(value); break;
      case eosio::name("dval2"):        this->dval2             = to_decimal<double>(value); break;
      case eosio::name("uival1"):       this->uival1            = to_integer<uint32_t>(value); break;
      case eosio::name("uival2"):       this->uival2            = to_integer<uint32_t>(value); break;
      case eosio::name("master"):       this->master            = to_name(value); break;
      default:
        eosio::check(false, "invalid config key");
    }
  }

private:

  template<typename T>
  inline T to_integer(const std::string &s){
    // return (T)std::stoul(s);   // std::stoul takes out the sign, so better use atoi and casting
    return (T)atoi(s.c_str());
  }

  template<typename T>
  inline T to_decimal(const std::string &s) {
    // return std::stod(s);   // std::stod doesn't work, as people say online, so implement our own
    std::size_t i = s.find(".");
    int digits = s.length() - i - 1;
    std::string v(s);
    v.erase(i, 1); 
    return (T)atoi(v.c_str()) / pow(10, digits);
  }

  inline eosio::name to_name(const std::string &s) { return eosio::name(s); }

};

typedef eosio::singleton<eosio::name("config"), config_data> config_singleton;

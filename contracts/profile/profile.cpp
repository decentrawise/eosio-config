#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/transaction.hpp>

#include "../../src/config.hpp"


class [[eosio::contract]] profile: public eosio::contract {

public:

  profile(eosio::name receiver, eosio::name code, eosio::datastream<const char*> ds) :
    contract(receiver, code, ds),
    config(receiver, receiver.value)
  {
    // load config
    cfg = config.get_or_create(receiver);
  }

  ~profile() {
    // save configuration
    config.set(cfg, eosio::same_payer);
  }

  [[eosio::action]]
  void update(eosio::name user, std::string nickname, std::string avatar,
              std::string website, std::string locale, std::string metadata) {
    require_auth(user);

    profiles_table profiles(_self, _self.value);
    auto iterator = profiles.find(user.value);
    if (iterator == profiles.end()) {
      // Create new
      profiles.emplace(user, [&](auto & row) {
        row.user = user;
        row.nickname = nickname;
        row.avatar = avatar;
        row.website = website;
        row.locale = locale;
        row.metadata = metadata;
      });
    } else {
      // Update existing
      profiles.modify(iterator, eosio::same_payer, [&](auto & row) {
        row.user = user;
        row.nickname = nickname;
        row.avatar = avatar;
        row.website = website;
        row.locale = locale;
        row.metadata = metadata;
      });
    }
  }

  [[eosio::action]]
  void remove(eosio::name user) {
    require_auth(user);

    profiles_table profiles(_self, _self.value);
    auto iterator = profiles.find(user.value);
    eosio::check(iterator != profiles.end(), "User doesn't have a profile");
    profiles.erase(iterator);
  }

  [[eosio::action]]
  void count() {
    profiles_table profiles(_self, _self.value);
    auto iterator = profiles.begin();
    eosio::check(iterator != profiles.end(), "No user profiles yet");
    // limit to 20 to check that an assert in the recurring action doesn't break the chain...
    eosio::check(iterator->count < 20, "Limit of 20 reached");
    profiles.modify(iterator, eosio::same_payer, [&](auto & row) {
      row.count++;
    });
  }

  [[eosio::action]]
  void configure(eosio::name key, std::string value) {
    // TODO: define who has access to perform configuration, ideally a msig account
    // require_auth(controlling_account);

    cfg.set(key, value);
  }

protected:
  struct [[eosio::table]] profile_entry {
    eosio::name user;
    std::string nickname;
    std::string avatar;
    std::string website;
    std::string locale;
    std::string metadata;
    uint64_t count = 0;

    uint64_t primary_key() const { return user.value; }
    uint64_t by_count() const { return count; }
  };
  typedef eosio::multi_index<eosio::name("profiles"), profile_entry> profiles_table;

  config_singleton config;  // the configuration singleton, do not use directly
  config_data cfg;
};

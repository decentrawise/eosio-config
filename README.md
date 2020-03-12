# eosio-config
A simple way to handle configuration values for EOSIO contracts and tests built with eoslime

## How it works

The only thing needed is to plug the config header file. Then on your contract constructor load the singleton config data and on the destructor save it.

```
#include "config.hpp"

class [[eosio::contract]] profile: public eosio::contract {

  config_singleton config;  // the configuration singleton, do not use directly
  config_data cfg;

  ...

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
  void ...
```

Then you just need to add the action to configure your contract... Please refer to the included test contract for a complete working example, like:

```
  [[eosio::action]]
  void configure(eosio::name key, std::string value) {
    // TODO: define who has access to perform configuration, ideally a msig account
    // require_auth(controlling_account);

    cfg.set(key, value);
  }
```

Throughout the code all configuration is accessible through the `cfg` member variable...

## File tree

- src/ - source directory that contains `config.hpp` main project code file

- contracts/ - test contract directory

- tests/ - tests specs

- scripts/ - helper scripts for running nodeos locally

## Install EOSLime
```bash
$ npm install -g eoslime
```

## Compile the example contract
```bash
$ eoslime compile
```

## Run a local EOSIO node
```bash
$ ./scripts/nodeos.sh
```
**NOTE**: Please customize the script to your local development needs. This might be made
easier in the future with configuration and a better script...

## Run tests
```bash
$ eoslime test
```

// Copyright 2020 <boikov3>
#include <DBHashCreator.hpp>
#include <constants.hpp>
#include <logs.hpp>

int main(int argc, char **argv) {
    po::options_description desc("short description");
    desc.add_options()
            ("help,h", "0 помощи")
            ("log_level", po::value<std::string>(),
             "level logging")
            ("thread_count", po::value<unsigned>(),
             "count of threads")
            ("output", po::value<std::string>(),
             "path result");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }

    catch (po::error &e) {
        std::cout << e.what() << std::endl;
        std::cout << desc << std::endl;
        return 1;
    }
    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return 1;
    }
    if (!vm.count("log_level")
        || !vm.count("thread_count")
        || !vm.count("output")) {
      std::cout << "error: bad format" << std::endl << desc << std::endl;
      return 1;
    }
    std::string logLVL = vm["log_level"].as<std::string>();
    std::size_t threadCount = vm["thread_count"].as<unsigned>();
    std::string pathToFile = vm["output"].as<std::string>();
    //std::cout << logLVL << " " << threadCount << " " << pathToFile;
    logs::logInFile();
    DBHashCreator db(pathToFile, threadCount, logLVL);
    db.startThreads();
    return 0;
}
/*
#include <cstdio>
#include <iostream>
#include <string>

#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"

std::string kDBPath = "/home/ivan/rocksdb_simple_example";

int main() {
  rocksdb::DB* db;
  rocksdb::Options options;
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  //options.IncreaseParallelism();
  //options.OptimizeLevelStyleCompaction();
  // create the DB if it's not already present
  options.create_if_missing = true;

  // open DB
  rocksdb::Status s = rocksdb::DB::Open(options, kDBPath, &db);
  if(!s.ok()) std::cerr << s.ToString() << std::endl;

  // Put key-value
  s = db->Put(rocksdb::WriteOptions(), "key1", "value");
  assert(s.ok());
  std::string value;
  // get value
  s = db->Get(rocksdb::ReadOptions(), "key1", &value);
  assert(s.ok());
  std::cout << value;
  assert(value == "value");

  // atomically apply a set of updates
  {
    rocksdb::WriteBatch batch;
    batch.Delete("key1");
    batch.Put("key2", value);
    s = db->Write(rocksdb::WriteOptions(), &batch);
  }

  s = db->Get(rocksdb::ReadOptions(), "key1", &value);
  assert(s.IsNotFound());

  db->Get(rocksdb::ReadOptions(), "key2", &value);
  assert(value == "value");

  {
    rocksdb::PinnableSlice pinnable_val;
    db->Get(rocksdb::ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
    assert(pinnable_val == "value");
  }

  {
    std::string string_val;
    // If it cannot pin the value, it copies the value to its internal buffer.
    // The intenral buffer could be set during construction.
    rocksdb::PinnableSlice pinnable_val(&string_val);
    db->Get(rocksdb::ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
    assert(pinnable_val == "value");
    // If the value is not pinned, the internal buffer must have the value.
    assert(pinnable_val.IsPinned() || string_val == "value");
  }

  rocksdb::PinnableSlice pinnable_val;
  db->Get(rocksdb::ReadOptions(), db->DefaultColumnFamily(), "key1", &pinnable_val);
  assert(s.IsNotFound());
  // Reset PinnableSlice after each use and before each reuse
  pinnable_val.Reset();
  db->Get(rocksdb::ReadOptions(), db->DefaultColumnFamily(), "key2", &pinnable_val);
  assert(pinnable_val == "value");
  pinnable_val.Reset();
  // The Slice pointed by pinnable_val is not valid after this point

  delete db;

  return 0;
}
 */
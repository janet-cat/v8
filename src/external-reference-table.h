// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_EXTERNAL_REFERENCE_TABLE_H_
#define V8_EXTERNAL_REFERENCE_TABLE_H_

#include <vector>

#include "src/address-map.h"
#include "src/builtins/builtins.h"
#include "src/external-reference.h"

namespace v8 {
namespace internal {

class Isolate;

// ExternalReferenceTable is a helper class that defines the relationship
// between external references and their encodings. It is used to build
// hashmaps in ExternalReferenceEncoder and ExternalReferenceDecoder.
class ExternalReferenceTable {
 public:
  static ExternalReferenceTable* instance(Isolate* isolate);

  // For the nullptr ref, see the constructor.
  static constexpr int kSpecialReferenceCount = 1;
  static constexpr int kExternalReferenceCount =
      ExternalReference::kExternalReferenceCount;
  static constexpr int kBuiltinsReferenceCount =
#define COUNT_C_BUILTIN(...) +1
      BUILTIN_LIST_C(COUNT_C_BUILTIN);
#undef COUNT_C_BUILTIN
  static constexpr int kRuntimeReferenceCount =
      Runtime::kNumFunctions / 2;  // Don't count dupe kInline... functions.
  static constexpr int kIsolateAddressReferenceCount = kIsolateAddressCount;
  static constexpr int kAccessorReferenceCount =
      Accessors::kAccessorInfoCount + Accessors::kAccessorSetterCount;
  // The number of stub cache external references, see AddStubCache.
  static constexpr int kStubCacheReferenceCount = 12;
  static constexpr int kSize =
      kSpecialReferenceCount + kExternalReferenceCount +
      kBuiltinsReferenceCount + kRuntimeReferenceCount +
      kIsolateAddressReferenceCount + kAccessorReferenceCount +
      kStubCacheReferenceCount;

  uint32_t size() const { return static_cast<uint32_t>(kSize); }
  Address address(uint32_t i) { return refs_[i].address; }
  const char* name(uint32_t i) { return refs_[i].name; }

  static const char* ResolveSymbol(void* address);

 private:
  struct ExternalReferenceEntry {
    Address address;
    const char* name;

    ExternalReferenceEntry() : address(nullptr), name(nullptr) {}
    ExternalReferenceEntry(Address address, const char* name)
        : address(address), name(name) {}
  };

  explicit ExternalReferenceTable(Isolate* isolate);

  void Add(Address address, const char* name, int* index);

  void AddReferences(Isolate* isolate, int* index);
  void AddBuiltins(Isolate* isolate, int* index);
  void AddRuntimeFunctions(Isolate* isolate, int* index);
  void AddIsolateAddresses(Isolate* isolate, int* index);
  void AddAccessors(Isolate* isolate, int* index);
  void AddStubCache(Isolate* isolate, int* index);

  ExternalReferenceEntry refs_[kSize];
  DISALLOW_COPY_AND_ASSIGN(ExternalReferenceTable);
};

}  // namespace internal
}  // namespace v8
#endif  // V8_EXTERNAL_REFERENCE_TABLE_H_

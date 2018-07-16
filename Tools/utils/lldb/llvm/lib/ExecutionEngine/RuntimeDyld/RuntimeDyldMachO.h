//===-- RuntimeDyldMachO.h - Run-time dynamic linker for MC-JIT ---*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// MachO support for MC-JIT runtime dynamic linker.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_RUNTIME_DYLD_MACHO_H
#define LLVM_RUNTIME_DYLD_MACHO_H

#include "llvm/ADT/IndexedMap.h"
#include "llvm/Object/MachOObject.h"
#include "llvm/Support/Format.h"
#include "RuntimeDyldImpl.h"

using namespace llvm;
using namespace llvm::object;


namespace llvm {
class RuntimeDyldMachO : public RuntimeDyldImpl {

  // For each section, keep a list of relocatable pieces of data that
  // reside in it.  If the section moves, or the sections whose
  // locations the data depends on move, re-resolve the relocations
  // based on that movement.
  //
  // RelocationEntry structures correspond to one or two Mach-O
  // relocation_info or scattered_relocation_info structures --
  // usually one, but two iff the original has a paired structure
  // following it.
  //
  // To facilitate updating a relocation when its sources move, we
  // also keep RelocationSource structures associated with the sections
  // whose location the data depends on.

  // FIXME: Use SymbolLoc for this instead.  Where should the enum live?
  struct SectionOffset {
    uint64_t    Offset;     // Offset of the location into its section.
    unsigned    ID;         // The section the location is contained in.

    enum {
      NoSectionID        = 0xffff0000,
      UnresolvedSourceID = 0xffffffff
    };
  };

  struct RelocationEntry {
    SectionOffset Sources[2]; // The section/offset pairs this relocation
                              // refers to.
                              // If the original Mach-O relocation entries used
                              // relocation_info, this data is computed from
                              // r_symbolnum and the offsets are locked to 0.
                              // (The only offset is determined by the addend.)
                              // If the original Mach-O relocation entries used
                              // scattered_relocation_info, this data, including
                              // offsets, is computed by looking r_value up in
                              // the section table.

    uint64_t      Offset;     // The offset of the data to be relocated.
                              // We don't use a SectionOffset because this
                              // RelocationEntry is already associated with the
                              // proper Section.

    int64_t       Addend;     // Addend encoded in the instruction itself, if any,
                              // plus the offset into the source section for
                              // the symbol once the relocation is resolvable.

    uint32_t      Data;       // If the original Mach-O relocation entry was a
                              // relocation_info, the bitfield { r_symbolnum,
                              // r_pcrel, r_length, r_extern, r_type }.
                              // If the original Mach-O relocation entry was a
                              // scattered_relocation_info, the bitfield
                              // { r_address, r_type, r_length, r_pcrel,
                              // r_scattered }.

    bool          Scattered;  // True iff this relocation is scattered.

    bool isPCRel()
    {
      if (Scattered)
        return (Data & 0x40000000) >> 30;
      else
        return (Data & 0x01000000) >> 24;
    }

    uint8_t type()
    {
      if (Scattered)
        return (Data & 0x0f000000) >> 24;
      else
        return (Data & 0xf0000000) >> 28;
    }

    // Returns the decoded version of the length field
    uint8_t length()
    {
      if (Scattered)
        return 1 << ((Data & 0x30000000) >> 28);
      else
        return 1 << ((Data & 0x0e000000) >> 25);
    }

    // Used with an ordinary relocation entry, where the source_offsets are not
    // known yet.
    RelocationEntry(uint64_t offset,      // See the Offset field.
                    unsigned source_id0,  // The section ID for the first source.
                    unsigned source_id1,  // The section ID for the second source.
                    uint32_t data,        // See the Data field.
                    int64_t addend)       // See the Addend field.
      : Offset(offset),
        Addend(addend),
        Data(data),
        Scattered(false) {
      Sources[0].ID = source_id0;
      Sources[0].Offset = 0;
      Sources[1].ID = source_id1;
      Sources[1].Offset = 0;
    }
  
    // Used with a scattered relocation entry, where the source_offsets can be
    // derived from the value.
    RelocationEntry(uint64_t offset,      // See the Offset field.
                    unsigned source_id0,  // The section ID for the first source.
                    uint64_t source_off0, // The offset for the first source.
                    unsigned source_id1,  // The section ID for the second source.
                    uint64_t source_off1, // The offset for the second source.
                    uint32_t data,        // See the Data field.
                    int64_t addend)       // See the Addend field.
      : Offset(offset),
        Addend(addend),
        Data(data), 
        Scattered(true) {
      Sources[0].ID = source_id0;
      Sources[0].Offset = source_off0;
      Sources[1].ID = source_id1;
      Sources[1].Offset = source_off1;
    }
  };
  typedef SmallVector<RelocationEntry, 4> RelocationList;

  // For each section, keep a list of sources that are used by relocations in
  // other sections.  Whenever a relocation gets created, create one or two
  // corresponding relocation sources.  Whenever relocations are re-resolved
  // for a section, also re-resolve the relocations corresponding to that
  // section's relocation targets.
  struct RelocationSource {
    unsigned    SectionID;      // Section whose RelocationList contains the relocation.
    uint32_t    Index : 24;     // Index of the RelocatonEntry in that RelocationList.
    uint8_t     SourceIdx : 1;  // Index of this source in the RelocationEntry's Sources.

    RelocationSource(unsigned id,
                     uint32_t index,
                     uint8_t source_idx)
      : SectionID(id),
        Index(index),
        SourceIdx(source_idx) {}
  };
  typedef SmallVector<RelocationSource, 4> RelocationSourceList;

  // Relocations which refer to already-loaded section. Indexed by SectionID
  // which is the section containing the relocatable data.
  IndexedMap<RelocationList> Relocations;
  // Targets corresponding to Relocations.
  IndexedMap<RelocationSourceList> RelocationSources;
  // Relocations to symbols that are not yet resolved. Must be external
  // relocations by definition. Indexed by symbol name.
  StringMap<RelocationSourceList> UnresolvedRelocations;

  bool resolveRelocationEntry(unsigned SectionID,
                              RelocationEntry &RE);
  bool resolveRelocation(uint8_t *LocalAddress,
                         uint64_t FinalAddress,
                         uint64_t FinalSource1,
                         uint64_t FinalSource2,
                         bool isPCRel,
                         unsigned Type,
                         unsigned Size,
                         int64_t Addend);
  bool resolveI386Relocation(uint8_t *LocalAddress,
                             uint64_t FinalAddress,
                             uint64_t FinalSource1,
                             uint64_t FinalSource2,
                             bool isPCRel,
                             unsigned Type,
                             unsigned Size,
                             int64_t Addend);
  bool resolveX86_64Relocation(uint8_t *LocalAddress,
                               uint64_t FinalAddress,
                               uint64_t FinalSource1,
                               uint64_t FinalSource2,
                               bool isPCRel,
                               unsigned Type,
                               unsigned Size,
                               int64_t Addend);
  bool resolveARMRelocation(uint8_t *LocalAddress,
                            uint64_t FinalAddress,
                            uint64_t FinalSource1,
                            uint64_t FinalSource2,
                            bool isPCRel,
                            unsigned Type,
                            unsigned Size,
                            int64_t Addend);

  bool loadSegment32(const MachOObject *Obj,
                     const MachOObject::LoadCommandInfo *SegmentLCI,
                     const InMemoryStruct<macho::SymtabLoadCommand> &SymtabLC);
  bool loadSegment64(const MachOObject *Obj,
                     const MachOObject::LoadCommandInfo *SegmentLCI,
                     const InMemoryStruct<macho::SymtabLoadCommand> &SymtabLC);
  bool processSymbols32(const MachOObject *Obj,
                      SmallVectorImpl<unsigned> &SectionMap,
                      SmallVectorImpl<StringRef> &SymbolNames,
                      const InMemoryStruct<macho::SymtabLoadCommand> &SymtabLC);
  bool processSymbols64(const MachOObject *Obj,
                      SmallVectorImpl<unsigned> &SectionMap,
                      SmallVectorImpl<StringRef> &SymbolNames,
                      const InMemoryStruct<macho::SymtabLoadCommand> &SymtabLC);

  void resolveSymbol(StringRef Name);

public:
  RuntimeDyldMachO(RTDyldMemoryManager *mm) : RuntimeDyldImpl(mm) {}

  bool loadObject(MemoryBuffer *InputBuffer);

  void reassignSectionAddress(unsigned SectionID, uint64_t Addr);

  static bool isKnownFormat(const MemoryBuffer *InputBuffer);

  bool isCompatibleFormat(const MemoryBuffer *InputBuffer) const {
    return isKnownFormat(InputBuffer);
  }
};

} // end namespace llvm

#endif

//===-- ABIMacOSX_arm64.h ---------------------------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef liblldb_ABIMacOSX_arm64_h_
#define liblldb_ABIMacOSX_arm64_h_

// C Includes
// C++ Includes
// Other libraries and framework includes
// Project includes
#include "lldb/lldb-private.h"
#include "lldb/Core/ConstString.h"
#include "lldb/Target/ABI.h"

class ABIMacOSX_arm64 :
    public lldb_private::ABI
{
public:
    ~ABIMacOSX_arm64() { }
    
    virtual size_t 
    GetRedZoneSize () const;
    
    virtual bool
    PrepareTrivialCall (lldb_private::Thread &thread,
                        lldb::addr_t sp,
                        lldb::addr_t functionAddress,
                        lldb::addr_t returnAddress,
                        llvm::ArrayRef<lldb::addr_t> args) const;

    virtual bool
    GetArgumentValues (lldb_private::Thread &thread,
                       lldb_private::ValueList &values) const;
    
    virtual bool
    CreateFunctionEntryUnwindPlan (lldb_private::UnwindPlan &unwind_plan);
    
    virtual bool
    CreateDefaultUnwindPlan (lldb_private::UnwindPlan &unwind_plan);
    
    virtual bool
    RegisterIsVolatile (const lldb_private::RegisterInfo *reg_info);
    
    
    virtual bool
    StackUsesFrames ()
    {
        // MacOSX uses frame pointers.
        return true;
    }
    
    virtual bool
    CallFrameAddressIsValid (lldb::addr_t cfa)
    {
        // Make sure the stack call frame addresses are are 16 byte aligned
        if (cfa & (16ull - 1ull))
            return false;   // Not 16 byte aligned
        if (cfa == 0)
            return false;   // Zero is not a valid stack address
        return true;
    }
    
    virtual bool
    CodeAddressIsValid (lldb::addr_t pc)
    {
        if (pc & (4ull - 1ull))
            return false;   // Not 4 byte aligned
        
        // Anything else if fair game..
        return true;
    }

    virtual bool
    FunctionCallsChangeCFA ()
    {
        return false;
    }

    virtual const lldb_private::RegisterInfo *
    GetRegisterInfoArray (uint32_t &count);

    //------------------------------------------------------------------
    // Static Functions
    //------------------------------------------------------------------
    static void
    Initialize();
    
    static void
    Terminate();
    
    static lldb::ABISP
    CreateInstance (const lldb_private::ArchSpec &arch);
    
    //------------------------------------------------------------------
    // PluginInterface protocol
    //------------------------------------------------------------------
    static lldb_private::ConstString
    GetPluginNameStatic();

    virtual lldb_private::ConstString
    GetPluginName()
    {
        return GetPluginNameStatic();
    }

    virtual const char *
    GetShortPluginName();
    
    virtual uint32_t
    GetPluginVersion();
    
    virtual lldb_private::Error
    SetReturnValueObject(lldb::StackFrameSP &frame_sp, lldb::ValueObjectSP &new_value);

protected:
    virtual lldb::ValueObjectSP
    GetReturnValueObjectImpl (lldb_private::Thread &thread,
                    lldb_private::ClangASTType &ast_type) const;

private:
    ABIMacOSX_arm64() : 
        lldb_private::ABI() 
    {
        // Call CreateInstance instead.  
    } 
};

#endif  // liblldb_ABI_h_

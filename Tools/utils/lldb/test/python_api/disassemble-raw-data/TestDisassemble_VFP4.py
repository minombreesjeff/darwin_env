"""
Use lldb Python API to disassemble raw machine code bytes
"""

import os, time
import re
import unittest2
import lldb, lldbutil
from lldbtest import *

class Disassemble_VFP4(TestBase):

    mydir = os.path.join("python_api", "disassemble-raw-data")

    @python_api_test
    def test_disassemble_vfp4_raw_data(self):
        """Test disassembling vfp4 raw bytes with the API."""
        self.disassemble_vfp4_raw_data()

    def disassemble_vfp4_raw_data(self):
        """Test disassembling vfp4 raw bytes with the API."""
        # Create a target from the debugger.

        # Try thumbv7s mode.
        target_thumb = self.dbg.CreateTargetWithFileAndTargetTriple ("", "thumbv7s-apple-ios")
        self.assertTrue(target_thumb, VALID_TARGET)

        raw_bytes = bytearray([0xe2, 0xee, 0xa1, 0x0b, # vfma.f64	d16, d18, d17
                               0xa2, 0xee, 0x00, 0x1a  # vfma.f32	s2, s4, s0
                               ])

        insts = target_thumb.GetInstructions(lldb.SBAddress(), raw_bytes)

        if self.TraceOn():
            print
            print "Raw bytes:    ", [hex(x) for x in raw_bytes]
            for i in insts:
                print "Disassembled%s" % str(i)
        for i in insts:
            self.assertTrue("vfma" in i.GetMnemonic(target_thumb))
 

if __name__ == '__main__':
    import atexit
    lldb.SBDebugger.Initialize()
    atexit.register(lambda: lldb.SBDebugger.Terminate())
    unittest2.main()

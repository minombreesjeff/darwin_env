"""
Use lldb Python API to disassemble raw machine code bytes
"""

import os, time
import re
import unittest2
import lldb, lldbutil
from lldbtest import *

class Disassemble_SDIV_UDIV(TestBase):

    mydir = os.path.join("python_api", "disassemble-raw-data")

    @python_api_test
    def test_disassemble_sdiv_udiv_raw_data(self):
        """Test disassembling sdiv/udiv raw bytes with the API."""
        self.disassemble_sdiv_udiv_raw_data()

    def disassemble_sdiv_udiv_raw_data(self):
        """Test disassembling sdiv/udiv raw bytes with the API."""
        # Create a target from the debugger.

        # Try thumbv7s mode.
        target_thumb = self.dbg.CreateTargetWithFileAndTargetTriple ("", "thumbv7-apple-ios")
        self.assertTrue(target_thumb, VALID_TARGET)

        raw_bytes = bytearray([0xb4, 0xfb, 0xf5, 0xf6, # udiv	r6, r4, r5
                               0x94, 0xfb, 0xf5, 0xf6  # sdiv	r6, r4, r5
                               ])

        insts = target_thumb.GetInstructions(lldb.SBAddress(), raw_bytes)

        if self.TraceOn():
            print
            print "Raw bytes:    ", [hex(x) for x in raw_bytes]
            for i in insts:
                print "Disassembled%s" % str(i)
        for i in insts:
            self.assertTrue("div" in i.GetMnemonic(target_thumb))
 
        # Try armv7s mode.
        target_arm = self.dbg.CreateTargetWithFileAndTargetTriple ("", "armv7-apple-ios")
        self.assertTrue(target_arm, VALID_TARGET)

        raw_bytes = bytearray([0x14, 0xf5, 0x16, 0xe7, # sdiv	r6, r4, r5
                               0x14, 0xf5, 0x36, 0xe7  # udiv	r6, r4, r5
                               ])

        insts = target_arm.GetInstructions(lldb.SBAddress(), raw_bytes)

        if self.TraceOn():
            print
            print "Raw bytes:    ", [hex(x) for x in raw_bytes]
            for i in insts:
                print "Disassembled%s" % str(i)
        for i in insts:
            self.assertTrue("div" in i.GetMnemonic(target_arm))


if __name__ == '__main__':
    import atexit
    lldb.SBDebugger.Initialize()
    atexit.register(lambda: lldb.SBDebugger.Terminate())
    unittest2.main()

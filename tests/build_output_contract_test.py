"""Exercise publication rules with a fake compiler, not Android code generation."""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

BUILDER=Path(__file__).resolve().parents[1]/'build_android.py'
class OutputContract(unittest.TestCase):
 def test_publication(self):
  with tempfile.TemporaryDirectory() as folder:
   root=Path(folder)
   (root/'build_android.py').write_bytes(BUILDER.read_bytes())
   for name in ['src','internal','expanded','host_libs']:(root/name).mkdir()
   (root/'src/sample.cpp').write_text('int sample;')
   cxx=root/'ndk/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi21-clang++'
   cxx.parent.mkdir(parents=True)
   cxx.write_text('''#!/usr/bin/env python3
import os,pathlib,sys
args=sys.argv[1:]
out=pathlib.Path(args[args.index('-o')+1])
if '-c' in args and os.environ.get('FAIL_STAGE')=='compile':sys.exit(1)
out.write_bytes(b'partial' if os.environ.get('FAIL_STAGE')=='link' else b'complete')
if '-shared' in args and os.environ.get('FAIL_STAGE')=='link':sys.exit(1)
''')
   cxx.chmod(0o755)
   out=root/'out';out.mkdir()
   final=out/'libInstrumentPannelLarge.so';pending=out/'libInstrumentPannelLarge.so.pending'
   hosts=['Common','GAdaptor','GAdaptorInterface','GFrame','GPlatformInterface','GSQuirrel','Gbl','base_utils','hsl']
   def run(stage):
    final.write_bytes(b'stale');pending.write_bytes(b'stale partial')
    return subprocess.run([sys.executable,str(root/'build_android.py'),'--ndk',str(root/'ndk'),'--out',str(out)],env=dict(os.environ,FAIL_STAGE=stage),capture_output=True)
   for stage in ['compile','hosts']:
    self.assertNotEqual(run(stage).returncode,0)
    self.assertFalse(final.exists());self.assertFalse(pending.exists())
   for name in hosts:(root/'host_libs'/('lib'+name+'.so')).touch()
   self.assertNotEqual(run('link').returncode,0)
   self.assertFalse(final.exists());self.assertFalse(pending.exists())
   result=run('success');self.assertEqual(result.returncode,0,result.stderr)
   self.assertEqual(final.read_bytes(),b'complete');self.assertFalse(pending.exists())
if __name__=='__main__':unittest.main()

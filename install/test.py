third_dlls_dir = ["E:/SHW/WorkSpace/3rdParty/tbb/lib", "E:/SHW/WorkSpace/3rdParty-src-win/install/GFE_API/lib"]
import os
for d in third_dlls_dir:
    os.add_dll_directory(d)


import GFE

db_path = "C:/Users/GZYL-11/Desktop/Model-1-DYNX-nonl0814-2.db"
db = GFE.Post.open(db_path, False)
print(GFE.Post.getNodeNum(db))
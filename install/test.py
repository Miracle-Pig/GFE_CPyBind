third_dlls_dir = ["E:/SHW/WorkSpace/3rdParty/tbb/lib", "E:/SHW/WorkSpace/3rdParty-src-win/install/GFE_API/lib"]
import os
for d in third_dlls_dir:
    os.add_dll_directory(d)


import GFE

db_path = "C:/Users/GZYL-11/Desktop/Model-1-DYNX-nonl0814-2.db"
db = GFE.Post.open(db_path, False)
print(GFE.Post.getNodeNum(db))

data = GFE.Post.FO.GetData(db, "U U1", True, 1)
print(data[1])
print(111)
grid_data = GFE.Post.FO.GetData_Node(db, "U U1", 1, False)
print(grid_data.GetGridSize())
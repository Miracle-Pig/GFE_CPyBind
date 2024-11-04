# third_dlls_dir = ["E:/SHW/WorkSpace/3rdParty/tbb/lib", "E:/SHW/WorkSpace/3rdParty-src-win/install/GFE_API/lib"]


# third_dlls_dir = ["../Extern/tbb/lib", "E:/SHW/WorkSpace/3rdParty-src-win/install/GFE_API/lib"]
import os
current_dir = os.path.dirname(os.path.abspath(__file__))
third_dlls_dir = [current_dir + "/../Extern/tbb/lib", "E:/SHW/WorkSpace/3rdParty-src-win/install/GFE_API/lib"]

for d in third_dlls_dir:
    os.add_dll_directory(d)


import GFE

db_path = "C:/Users/GZYL-11/Desktop/TestDB/ReportDB/减震罕遇模型及结果/Model-1-DYNY-nonl0814/Model-1-DYNY-nonl0814-2.db"
db = GFE.Post.open(db_path, False)
print(GFE.Post.getNodeNum(db))

data = GFE.Post.FO.GetData(db, "U U1", True, 1)
print(data[1])
grid_data = GFE.Post.FO.GetData_Node(db, "U U1", 1)
print(grid_data.GetGridSize())
print(grid_data.GetRange())
#!/usr/bin/env python3
import zipfile, pathlib
import os

workdir = pathlib.Path(__file__).parent.absolute()
os.chdir(workdir)
print(f"Workdir: {os.getcwd()}")

hw_dir = os.listdir("./Homeworks")
for idx, hw_name in enumerate(hw_dir):
  print(f"{idx+1}: {hw_name}")

destHW = input("Please specify work to pack: ")
destIdx = int(destHW) - 1
print(f"You've specified {destIdx + 1}: {hw_dir[destIdx]}")

destZipName = f"46_刘紫檀_Homework{destIdx + 1}.zip"

def zipDir(ziph, path):
  excludes = ['.git']

  for root, dirs, files in os.walk(path, topdown=True):
    dirs[:] = [d for d in dirs if d not in excludes]
    for file in files:
      ziph.write(os.path.join(root, file), 
        os.path.relpath(os.path.join(root, file), 
                        os.path.join(path, '..')))

with zipfile.ZipFile(destZipName, 'x', compression=zipfile.ZIP_DEFLATED, compresslevel=9) as zipInst:
  zipDir(zipInst, f"./Homeworks/{hw_dir[destIdx]}/")
  zipInst.write(f"Reports-PDF/HW{destIdx + 1}.pdf", f"46_Homework{destIdx + 1}.PDF")


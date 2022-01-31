from anytree.importer import DictImporter
from anytree import RenderTree
from anytree.exporter import DotExporter, UniqueDotExporter
import json
import os

files = os.listdir('../build')
if(os.path.isdir("../visualizations")==False):
    os.mkdir("../visualizations")
for file in files:
    if file.endswith(".json"):
        try:
            importer = DictImporter()
            data = json.load(open("../build/"+file))
            tree = importer.import_(data)
            UniqueDotExporter(tree).to_picture("../visualizations/"+file.split(".")[0]+".png")
            print(RenderTree(tree))
        except Exception as e:
            #print(e)
            pass

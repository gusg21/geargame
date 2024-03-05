import yaml

f = open("fontManifest.yaml", "r")
ydata = yaml.load(f, yaml.FullLoader)
f.close()

print("#ifndef ICONS_H")
print("#define ICONS_H")
print("")
print("#include <stdint.h>\n")

for g in ydata["glyphs"]:
    print("#define ICON_{} \"\\u{}\"".format(g["name"].upper().replace("-", "_"), g["code"][2:]))

print("")
print("#endif // ICONS_H")
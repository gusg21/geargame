import yaml

f = open("MaterialIcons-Regular.codepoints", "r")
text = f.readlines()
f.close()

print("#ifndef ICONS_H")
print("#define ICONS_H")
print("")

max = 0
min = 1000000
count = 0

for line in text:
    p = line.strip().split(" ")
    name = p[0]
    point = p[1]
    print("#define ICON_{} \"\\u{}\"".format(name.upper(), point))

    num_pt = int(point, 16)
    if num_pt > max:
        max = num_pt
    if num_pt < min:
        min = num_pt

    count += 1

print("")
print("#define MIN_CODE_POINT {}".format(min))
print("#define MAX_CODE_POINT {}".format(max))
print("#define NUM_CODE_POINT {}".format(count))
print("")
print("#endif // ICONS_H")
    
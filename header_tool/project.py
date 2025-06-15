import export_variables
import json
import sys

print("Reading file:", sys.argv)

variables = export_variables.export_variables(sys.argv[1])

print("")
print("### Found variables ###")

actor_json = {"exported_variables" : []}
class_name = ""

for variable in variables:
    print("-------------------------")
    
    actor_json["exported_variables"].append({
        "data_type" : variable.data_type,
        "name" : variable.name,
        "default_value" : variable.default_value,
        "attributes" : [{"name" : attribute.name, "value" : attribute.values} for attribute in variable.attributes]
    })

    if variable.data_type == 'class':
        class_name = variable.name
        print("Found class", variable.name, "inheriting from", variable.default_value)

    print("Attributes:")
    for attribute in variable.attributes:
        if attribute != None: print("    ", attribute.name, attribute.values)
    print("Variable:",variable.data_type, variable.name, variable.default_value)

if class_name == "":
    print("[x] UFO-Project Manager: Error, no class found")
    sys.exit()

f = open(sys.argv[1]+"_exported_variables.json", "w")

f.write(json.dumps(actor_json, indent=4))
f.close()

print(actor_json)
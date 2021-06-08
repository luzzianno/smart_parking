import json

def Average(lst):
    return sum(lst) / len(lst)

dataX = []
dataY = []
with open('cerca_batery_rueda1.json') as f:
    data = json.load(f)

print(data)

for i in data['datos']:
    dataX.append(i['X'])
    dataY.append(i['Y'])

print(Average(dataX))
print(Average(dataY))

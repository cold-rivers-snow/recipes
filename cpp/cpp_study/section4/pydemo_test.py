import sys
from pathlib import Path
sys.path.append(str(Path(__file__).parent / "build"))

import pydemo

pydemo.info()
print(pydemo.add(1, 2))

s = pydemo.use_str("hello")
print(s)

lst = pydemo.use_list([1, 2, 3])
print(lst)

p = pydemo.Point(42)
print(p.get())
p.set(100)
print(p.get())
from future import print_function
import os

print("<html><head></head><body><h2>Hello,", end=' ')
print(os.environ['name'], end=' ')
print("from Python</h2></body></html>")
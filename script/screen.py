import ctypes
user32 = ctypes.windll.user32
screensize = user32.GetSystemMetrics(0), user32.GetSystemMetrics(1)

print(screensize)
#print ("width =", GetSystemMetrics (0))
#print ("height =",GetSystemMetrics (1))


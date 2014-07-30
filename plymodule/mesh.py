import math;

def f(x,y):
	return x*y #math.exp(-(x*x + y*y)/2*sigma*sigma)#/(2.0*3.1415*sigma*sigma)

def idx(i,j,ny):
	return i*ny + j

nx = 101
ny = 101

x0 = -1.0
y0 = -1.0

dx = 2.0/nx
dy = 2.0/ny

print "ply"
print "format ascii 1.0"
print "element vertex", (nx+1)*(ny+1)
print "property float x"
print "property float y"
print "property float z"
print "element face", nx*ny*2
print "property list uchar int vertex_indices"
print "end_header"

for i in range(nx+1):
	x = x0 + dx*i
	for j in range(ny+1):
		y = y0 + dy*j
		print x,y,0.0
	 
for i in range(nx):
	for j in range(ny):
		print 3, idx(i,   j,   ny+1), idx(i+1, j,   ny+1), idx(i+1, j+1, ny+1)
		print 3, idx(i+1, j+1, ny+1), idx(i,   j+1, ny+1), idx(i,   j,   ny+1)
		#print 4, idx(i,j,ny+1), idx(i+1,j,ny+1), idx(i+1,j+1,ny+1), idx(i,j+1,ny+1)

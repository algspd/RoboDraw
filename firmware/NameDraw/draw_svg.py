from xml.dom import minidom
import time


svgfile = "font.svg"

document = minidom.parse(svgfile)

path = document.getElementsByTagName("path")

#for e in path:
#    print e

e = path[0]

m = e.getAttribute("d")

l = m.split()

lps = [p for p in l if p!=u'm' and p!=u'z']

#-- List of points in relative coordinates. The firs one is the initial point in absolute
#-- coordinates
lp = [ (float(p.split(',')[0]), float(p.split(',')[1])) for p in lps ]

#print lp
#x0,y0 = lp[0]
#xc,yc = (-x0, -y0)
xc,yc = (0, 0)

alp = []

for p in lp:
    x,y = p
    xc += x
    yc += y
    alp.append((xc, yc))

alp = [(p[0], -p[1]) for p in alp]
minx=100000000
miny=100000000
for p in alp:
  minx=min(minx,p[0])
  miny=min(miny,p[1])
print minx
print miny
alp = [(p[0]-minx, p[1]-miny) for p in alp]

for p in alp:
  # Ir mostrando array C guardando anterior y si actual=anterior boli arriba=1
  # 



print alp
    
#-- Add the first point    
#alp.append(alp[0])



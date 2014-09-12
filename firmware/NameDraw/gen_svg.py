from xml.dom import minidom
import time


svgfile = "font.svg"

document = minidom.parse(svgfile)

path = document.getElementsByTagName("path")

#print 'typedef struct{'
#print '  float x;'
#print '  float y;'
#print '  char v;'
#print '} coordenada;'

#print '//Max 28x6 Si lo haces mas grande peta el arduino (se queda sin memoria?)'
#print 'coordenada alfabeto[53][24];'
#print 'int foo=5;'

#print 'void initletra(){'

print '#include <avr/pgmspace.h>'
print 'float const PROGMEM alfabeto[53][24][3]={'

j=0;

for e in path:
#    print e

  e = path[j]

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
  #print minx
  #print miny
  alp = [(p[0]-minx, p[1]-miny) for p in alp]
  
  print '  {'
  print '  //Letra numero {0}'.format(j+1)
  i=0
  lastp={0.432342,0.34352}
  v=0
  for p in alp:
    # Ir mostrando array C guardando anterior y si actual=anterior boli arriba=1
    # 
    if lastp == p:
      nextv=0
    else:
      nextv=1
      #print '  alfabeto[{letra}][{punto}]={{ {x}, {y}, {v} }};'.format(letra=j, punto=i, x=p[0], y=p[1], v=v)
      print '  {{ {x}, {y}, {v} }},'.format(letra=j, punto=i, x=p[0], y=p[1], v=v)
  
    v=nextv
    lastp=p
    i=i+1

  print '  },'
  j=j+1

#print alp
    
#-- Add the first point    
#alp.append(alp[0])

print "};"

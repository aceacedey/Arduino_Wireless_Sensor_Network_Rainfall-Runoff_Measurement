import sys
import json
import urllib.request
from urllib.request import Request, urlopen
from bs4 import BeautifulSoup
from pprint import pprint
import time
import csv
import os
prev = float(0)
start_time = time.clock()
end_time = time.clock()
newline = str()
full = str()
odate = time.strftime("%c")
omonth = odate[0] + odate[1]
oday = odate[3] + odate[4]

cmonth = odate[0] + odate[1]
cday = odate[3] + odate[4]
startpath = 'C:\Program Files\Apache Software Foundation\Apache2.2\htdocs\mapiitkgp.com\Data\VSRC'
first_dir = os.path.join(startpath, 'Rainfall %s-2015' % omonth)
if not os.path.exists(first_dir):
    os.makedirs(first_dir)
f_dir = str(first_dir)
#print(f_dir)
var = 1
while var == 1:
    if int(cmonth) == int(omonth):
        if int(cday) == int(oday):
            #print(int(oday))
            #print(int(cday))
            #time.sleep(900)
            cdate = time.strftime("%c")
            cday = cdate[3] + cdate[4]
            #print(f_dir)
            file = open(os.path.join(f_dir, "VSRC_%s.txt" %oday), "w")

            try:
                req = Request(('http://10.124.68.100'), headers={'User-Agent': 'Mozilla/5.0'})
                webpage = urlopen(req).read()
                rslt = BeautifulSoup(webpage)
                #print("webpage found")
                gtindex = rslt.get_text()
                temp = gtindex.splitlines()
                #print(temp)
                for i, line in enumerate (gtindex.splitlines()):
                    #print("webpage found")                
                    if line == (" Total RAINFALL in past 5 mintutes :"):
                        cur = float(temp[i+1])
                        #print(cur)
                        if (cur == float(0)) & (prev == float(0)):
                            end_time = time.clock()
                            if float(end_time) - float(start_time) > 300:
                                rainfall = cur
                                
                                print("rainfall to be printed")
                                print(rainfall)
                                newline = cdate + '\t' + str(rainfall) + '\n'
                                print(newline)
                                full = full + newline
                                newline = str()
                                start_time = time.clock()
                            else:
                                end_time = time.clock()
                        
                        elif (cur == float(0)) & (prev != float(0)):
                            end_time = time.clock()
                            if float(end_time) - float(start_time) > 300:
                                print("------rainfall to be printed")
                    
                                rainfall = prev
                                print(rainfall)
                                newline = cdate + '\t' + str(rainfall) +'\n'
                                print(newline)
                                full = full + newline
                                newline = str()
                                prev = cur
                                start_time = time.clock()
                            else:
                                end_time = time.clock()

                        elif (cur != float(0)) & (prev == float(0)):
                            end_time = time.clock()
                            if float(end_time) - float(start_time) > 300:
                                rainfall = cur
                                
                                print("rainfall to be printed")
                                print(rainfall)
                                newline = cdate + '\t' + str(rainfall) + '\n'
                                print(newline)
                                full = full + newline
                                newline = str()
                                prev = cur
                                start_time = time.clock()
                            else:
                                end_time = time.clock()
                        elif (cur != float(0)) & (prev != float(0)):
                            end_time = time.clock()
                            if float(end_time) - float(start_time) > 300:
                                rainfall = cur
                                
                                print("rainfall to be printed")
                                print(rainfall)
                                newline = cdate + '\t' + str(rainfall) + '\n'
                                print(newline)
                                full = full + newline
                                newline = str()
                                start_time = time.clock()
                            else:
                                end_time = time.clock()
                        else:
                            prev = cur

            except urllib.error.HTTPError as e:
                print(e.code)
                print(e.read())

            except:
                print("Unknown Error")
        
        else:
            
            file.write(full)            
            file.close()
            full = str()
            oday = cday
    else:
        newpath = 'C:\Program Files\Apache Software Foundation\Apache2.2\htdocs\mapiitkgp.com\Data\VSRC'
        _dir = os.path.join(newpath, 'Rainfall %s-2015' % omonth)
        if not os.path.exists(_dir):
            os.makedirs(_dir)
        f_dir = str(_dir)
        print(f_dir)
        omonth = cmonth

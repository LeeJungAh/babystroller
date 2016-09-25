import socket
import MySQLdb
import copy
import time


s = socket.socket()         # Create a socket object
host = '' # Get local machine name

myport = 65504               # Reserve a port for your service.

s.bind((host, myport)) 
s.listen(15)                 # Now wait for client connection.

def main():
  loop = True
  while loop:
    c, addr = s.accept()     # Establish connection with client.
    print 'Got connection from', addr
    db = MySQLdb.connect(host='127.0.0.1' ,user='root', passwd='Ekfrl13', db='babystroller')
      

    while loop:
      light = c.recv(8)
      print light
      gps = c.recv(8)
      print gps
      ultrasonic = c.recv(8)
      print ultrasonic
      with db:
        cur = db.cursor()
        cur.execute("INSERT INTO light(light) VALUES (%s)",[light])
        cur.execute("INSERT INTO gps(gps) VALUES (%s)",[gps])
        cur.execute("INSERT INTO ultrasonic(ultrasonic) VALUES (%s)",[ultrasonic])

    if data == "x":
      loop = False
      c.send('Thank you for connecting')
    c.close()
  pass


if __name__ == '__main__':
  main()

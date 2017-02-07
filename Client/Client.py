#Usage : Usage: ./client -n <team> -p <port> [-h <hostname>]
#-n nom d'equipe
#-p port
#-h nom de la machine par defaut c'est le localhost

from SocketClass import Socket
from IaClass import IA
import signal, argparse, sys, os

def     checkTeam(team):
        if not team.isalnum():
            print("[Syntax ERROR] : Invalid team name format.")
            sys.exit()

def     checkPort(port):
        if int(port) < 1 and int(port) > 65534:
            print("[Syntax ERROR] : Invalid Port format.")
            sys.exit()

def     checkIP(ip):
        #Check IP
        ip_s = ip.split('.')
        if ip != "localhost":
            if len(ip_s) != 4:
                print("[Syntax ERROR] : Invalid IP format.")
                sys.exit()
            for x in ip_s:
                if not x.isdigit():
                    print("[Syntax ERROR] : Invalid IP format.")
                    sys.exit()
                conv = int(x)
                if conv < 0 or conv > 255:
                    print("[Syntax ERROR] : Invalid IP format.")
                    sys.exit()

#Signal ctrl + c handler
def signal_handler(signal, frame):
	print('[Info] : Ctrl + c pressed. Good bye !')
	os._exit(0)

if __name__ == "__main__":

    #Init argparse.
    parser = argparse.ArgumentParser(add_help=False)

    #All My parsing args.
    parser.add_argument("-h", "--host", dest = "ip", default= "127.0.0.1", help="Ip")
    parser.add_argument("-p", "--port",dest ="port", help="port", type=int, required=True)
    parser.add_argument("-n", "--team",dest ="team", help="team name", type=str, required=True)

    #All stored args.
    args = parser.parse_args()
    checkIP(args.ip)
    checkPort(args.port)
    checkTeam(args.team)
    signal.signal(signal.SIGINT, signal_handler)
    sock = IA(args.ip, args.port, args.team)

import sys, os
from datetime import datetime
import dpkt

# main
argc = len(sys.argv)
pcap_fpath = sys.argv[1]
f = open(pcap_fpath, 'rb')
pcap = dpkt.pcap.Reader(f)
for ts, buf in pcap:
    eth = dpkt.ethernet.Ethernet(buf)
    if eth.type != dpkt.ethernet.ETH_TYPE_IP:
        continue
    ip = eth.data
    if ip.p == dpkt.ip.IP_PROTO_TCP :
        tcp = ip.data
        print('tcp', ts, len(tcp), len(tcp.data), ip.src, tcp.sport, '->', ip.dst, tcp.dport)
    elif ip.p == dpkt.ip.IP_PROTO_UDP :
        udp = ip.data
        print('udp', ts, len(udp), len(udp.data), ip.src, udp.sport, '->', ip.dst, udp.dport)
f.close()

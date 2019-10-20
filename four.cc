#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"
#include "ns3/ipv4-global-routing-helper.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("SECOND SCRIPT EXAMPLE");
int main(int argc,char *argv[]){
  bool verbose=true;
  uint32_t nCsma=3;
  CommandLine cmd;
  cmd.AddValue ("nCsma","Number of \"extra\" CSMA nodes/devices",nCsma);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

  nCsma = nCsma == 0 ? 1 : nCsma;
  NodeContainer nodes;
  nodes.Create(3);

  NodeContainer csmanodes;
  csmanodes.Add(nodes.Get(2));
  csmanodes.Create(nCsma);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointToPoint.SetChannelAttribute("Delay",StringValue("2ms"));
  NetDeviceContainer devices;
  devices=pointToPoint.Install(nodes.Get(0),nodes.Get(1));

  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointToPoint1.SetChannelAttribute("Delay",StringValue("2ms"));
  NetDeviceContainer devices1;
  devices1=pointToPoint.Install(nodes.Get(1),nodes.Get(2));

  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate",StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));
  NetDeviceContainer csmadevices;
  csmadevices=csma.Install(csmanodes);

  InternetStackHelper stack;
  stack.Install(nodes.Get(0));
  stack.Install(nodes.Get(1));
  stack.Install(csmanodes);

  Ipv4AddressHelper address;
  address.SetBase("20.1.1.0","255.255.255.0");
  Ipv4InterfaceContainer inter;
  inter=address.Assign(devices);

  Ipv4AddressHelper address1;
  address1.SetBase("20.1.1.0","255.255.255.0");
  Ipv4InterfaceContainer inter1;
  inter1=address.Assign(devices1);

  Ipv4AddressHelper address2;
  address2.SetBase("30.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer csmainterfaces;
  csmainterfaces=address2.Assign(csmadevices);

  UdpEchoServerHelper echoserver(9);
  ApplicationContainer serverapps;
  serverapps=echoserver.Install(csmanodes.Get(nCsma));
  serverapps.Start(Seconds(1.0));
  serverapps.Stop(Seconds(10.0));

  UdpEchoClientHelper echoclient(csmainterfaces.GetAddress(nCsma),9);
  echoclient.SetAttribute("MaxPackets",UintegerValue(1));
  echoclient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
  echoclient.SetAttribute("PacketSize",UintegerValue(1024));

  ApplicationContainer clientapps;
  clientapps=echoclient.Install(nodes.Get(0));
  clientapps.Start(Seconds(2.0));
  clientapps.Stop(Seconds(10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  Simulator::Run();
  Simulator::Destroy();
  return 0;
}

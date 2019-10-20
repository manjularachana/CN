#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/ipv4-global-routing-helper.h"
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("SecondScriptExample");
int main(int argc,char *argv[]){
  bool verbose=true;
  uint32_t nCsma=2;
  CommandLine cmd;
  cmd.AddValue("nCsma","Total number of\"extra\"CSMA nodes/devices",nCsma);
  cmd.AddValue("verbose","Tell the applications to log if true",verbose);
  cmd.Parse(argc,argv);
  if(verbose){
    LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
  }
  nCsma=nCsma==0?1:nCsma;
  NodeContainer nodes;
  nodes.Create(3);
  NodeContainer nodes2;
  nodes2.Create(3);

  NodeContainer csmaNodes;
  csmaNodes.Add(nodes.Get(2));
  csmaNodes.Create(nCsma);
  csmaNodes.Add(nodes2.Get(0));

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointToPoint.SetChannelAttribute("Delay",StringValue("2ms"));

  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointToPoint1.SetChannelAttribute("Delay",StringValue("2ms"));

  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointToPoint2.SetChannelAttribute("Delay",StringValue("2ms"));

  PointToPointHelper pointToPoint3;
  pointToPoint3.SetDeviceAttribute("DataRate",StringValue("5Mbps"));
  pointToPoint3.SetChannelAttribute("Delay",StringValue("2ms"));

  NetDeviceContainer p2pdevices,p2pdevices1,p2pdevices2,p2pdevices3;
  p2pdevices=pointToPoint.Install(nodes.Get(0),nodes.Get(1));
  p2pdevices1=pointToPoint1.Install(nodes.Get(1),nodes.Get(2));
  p2pdevices2=pointToPoint2.Install(nodes2.Get(0),nodes2.Get(1));
  p2pdevices3=pointToPoint3.Install(nodes2.Get(1),nodes2.Get(2));

  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate",StringValue("100Mbps"));
  csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)));

  NetDeviceContainer csmadevices;
  csmadevices=csma.Install(csmaNodes);

  InternetStackHelper stack;
  stack.Install(nodes.Get(0));
  stack.Install(nodes.Get(1));
  stack.Install(csmaNodes);
  stack.Install(nodes2.Get(1));
  stack.Install(nodes2.Get(2));


  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0","255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces=address.Assign(p2pdevices);

  address.SetBase("10.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces1;
  p2pInterfaces1=address.Assign(p2pdevices1);

  Ipv4AddressHelper address1;
  address1.SetBase("20.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces=address1.Assign(csmadevices);

  Ipv4AddressHelper address2;
    address2.SetBase("30.1.1.0","255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces2;
p2pInterfaces2=address2.Assign(p2pdevices2);

address2.SetBase("30.1.2.0","255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces3;
  p2pInterfaces3=address2.Assign(p2pdevices3);

//  p2pInterfaces2=address2.Assign(p2pdevices3);


  UdpEchoServerHelper echoServer(9),echoServer1(10);

  ApplicationContainer serverapps=echoServer.Install(csmaNodes.Get(nCsma));
  serverapps.Start(Seconds(1.0));
  serverapps.Stop(Seconds(10.0));

  ApplicationContainer echoserverapps1=echoServer1.Install(csmaNodes.Get(nCsma));
  echoserverapps1.Start(Seconds(1.0));
  echoserverapps1.Stop(Seconds(10.0));

  UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(nCsma),9);
  echoClient.SetAttribute("MaxPackets",UintegerValue(1));
  echoClient.SetAttribute("Interval",TimeValue(Seconds(1.0)));
  echoClient.SetAttribute("PacketSize",UintegerValue(1024));

  UdpEchoClientHelper echoClient1(csmaInterfaces.GetAddress(nCsma),10);
  echoClient1.SetAttribute("MaxPackets",UintegerValue(1));
  echoClient1.SetAttribute("Interval",TimeValue(Seconds(1.0)));
  echoClient1.SetAttribute("PacketSize",UintegerValue(1024));

  ApplicationContainer clientApps=echoClient.Install(nodes.Get(1));
  clientApps.Start(Seconds(2.0));
  clientApps.Stop(Seconds(10.0));

  ApplicationContainer clientApps2=echoClient1.Install(nodes2.Get(2));
  clientApps2.Start(Seconds(2.0));
  clientApps2.Stop(Seconds(10.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();

  Simulator::Run();
  Simulator::Destroy();
  return 0;
}

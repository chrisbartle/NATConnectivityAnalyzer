# NAT Connectivity Analyzer
<img width="377" height="727" alt="image" src="https://github.com/user-attachments/assets/70425a3d-d14d-40e6-a17d-c885c8abfe23" />
<img width="377" height="727" alt="image" src="https://github.com/user-attachments/assets/a0f95507-ac88-46c4-8bcb-0720ef14dd63" />

The NAT Connectivity Analyzer is a tool that be used to evaluate, diagnose, and explain your local Network Address Translation (NAT) device. NAT is a common feature on nearly every modern router. It can help diagnose problems related to peer-to-peer applications, such as videogames or videoconferencing.

## Features
- Uses RFC 3489 to determine the local NAT’s type (Full Cone, Restricted Cone, etc).
- Uses PMP, PCP, and UPnP to detect and test automatic port forwarding
  - Can distinguish between mono-directional and bidirectional forwarded ports.
  - Will attempt to confirm that the port forwarding works correctly.
- The software will flag double-NATs if detected.
- Explanatory text is provided for all of its findings, just press the ? button.
- No ads and no tracking. This software only connects to the STUN server that YOU provide.
- NAT Connectivity Analyzer is a standalone application. Just download and run!

## How It Works
The user is prompted to enter a STUN server. There combo box is pre-populated with a list of possible choices or the user to can enter a different address. Once the Start button has been pressed, the following steps are followed:
1. The software displays the machine's local IP addresses
2. The software queries the STUN server for the machine's external IP address
3. The software follows RFC 3489 to determine the type of NAT
4. The software attempts to initiate automatic port forwarding using PMP, PCP, and UPnP
   - If UPnP is enabled on the router than additional router information is collected and made available (Show Diagnostics must be checked)
5. The software communicates with the STUN server using the forwarded port. If the STUN server reveals that it is receiving traffic from the forwarded port then the port forwarding is known to be bidirectional, if it is coming from a different port then the forwarding must be monodirectional.
6. The software asks the STUN server to change its IP address and to send a packet back to the forwarded port. If this packet is received then the port forwarding is known to be working effectively.
   - Some STUN servers do not support sending packets to an alternative port. If the machine fails this test then re-run it with an alternative STUN server.

## Build Instructions
This software currently builds fine using QT Creator in Linux and Windows.

It may only compile in Windows using the MSVC compiler.

It is compiled statically for the release builds.


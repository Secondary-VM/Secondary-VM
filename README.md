# Secondary-VM

## Prerequisites 

- A host machine that is installed and enabled with the virtualization modules and packages.
- Suggested host/guest OS: Ubuntu 20.04 or newer, RHEL 8.5 or newer.
- A primary VM disk image that contains a [client daemon](https://github.com/Secondary-VM/Secondary-VM/blob/main/vm-example/secondary_vm.c).
- A secondary drive that is hotplugged to the primary VM and contains a secondary VM disk image. 

To run AMD-based confidential primary and secondary VMs, you need: 

- An AMD machine that supports SEV, SEV-ES, or/and SEV-SNP. 
- For host and guest configurations to enable SEV and run SEV VMs, please refer to https://github.com/AMDESE/AMDSEV/tree/master

## Steps to run a secVM demo

1. Compile the secVM-enabled libvirt code: https://github.com/Secondary-VM/libvirt
2. In your build environment, run the `virtlockd`, `virtlogd`, and `libvirtd` binaries.
3. Spawn a primary VM: `./virsh create Secondary-VM/vm-example/primaryVM01.xml`
4. Spawn a secondary VM by running `secondary_vm` commands in the primary VM. For example, if the primary VM requests to spawn/create an encrypted secondary VM with 2 vCPUs, 4 GB memory, and using `secVM1.img` raw disk image, the command is: `sudo ./secondary_vm -c 2 -m 4 -d secVM1.img -t raw -e 1 create`.

## What the [vm-example](https://github.com/Secondary-VM/Secondary-VM/tree/main/vm-example) folder contains:

- [primaryVM01.xml](https://github.com/Secondary-VM/Secondary-VM/blob/main/vm-example/primaryVM01.xml): an XML configuration sample file to create a primary VM.
- [secVmTemplate.xml](https://github.com/Secondary-VM/Secondary-VM/blob/main/vm-example/secVmTemplate.xml): an XML configuration template file to create a secondary VM. 
- [secondary_vm.c](https://github.com/Secondary-VM/Secondary-VM/blob/main/vm-example/secondary_vm.c): a client daemon that is deployed in the primary VM.

## License

Apache License 2.0

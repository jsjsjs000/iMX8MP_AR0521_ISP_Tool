	Install Linux SDK - NXP 23.1.0
# https://download.phytec.de/Software/Linux/BSP-Yocto-i.MX8MP/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sdk/ampliphy-vendor-xwayland
# download (2.1GB)
wget https://download.phytec.de/Software/Linux/BSP-Yocto-i.MX8MP/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sdk/ampliphy-vendor-xwayland/phytec-ampliphy-vendor-xwayland-glibc-x86_64-phytec-qt6demo-image-cortexa53-crypto-toolchain-4.0.13.sh
chmod +x phytec-ampliphy-vendor-xwayland-glibc-x86_64-phytec-qt6demo-image-cortexa53-crypto-toolchain-4.0.13.sh
# run installer
./phytec-ampliphy-vendor-xwayland-glibc-x86_64-phytec-qt6demo-image-cortexa53-crypto-toolchain-4.0.13.sh
# extract SDK to default SDK folder:
#		/opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0

# set up script:
# source /opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/environment-setup-cortexa53-crypto-phytec-linux
# set sysroot /opt/ampliphy-vendor-xwayland/BSP-Yocto-NXP-i.MX8MP-PD23.1.0/sysroots/cortexa53-crypto-phytec-linux

	Compile, upload to target and run program
Terminal > Uruchom zadanie kompilacji (Ctrl+Shift+B)

	Debug
Uruchom > Rozpocznij debugowanie (F5)

	Open debug port
sudo ufw allow 3000
sudo ufw reload
sudo ufw status

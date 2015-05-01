sudo rmmod uvcvideo
sudo modprobe uvcvideo nodrop=1 timeout=10000
echo 'video module loaded !'
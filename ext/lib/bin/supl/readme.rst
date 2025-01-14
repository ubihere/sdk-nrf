SUPL Client library
###################

This is the SUPL Client library module that integrates SUPL client library to NCS.
The GNSS sample located in nrf/samples/nrf9160/gnss has support for receiving A-GPS data over the SUPL protocol.

Downloading
***********
The SUPL client library zip file must be downloaded from https://www.nordicsemi.com/Products/Low-power-cellular-IoT/nRF9160.
Click the Downloads box, and you should find "nRF9160 SiP SUPL client library" at the bottom of the page.

Installing
**********
Unzip the downloaded zip file in nrf/ext/lib/bin/ while keeping the folder structure, final folder structure should be nrf/ext/lib/bin/supl/.
The support is enabled by adding line CONFIG_GNSS_SAMPLE_ASSISTANCE_SUPL=y to the prj.conf.
Also, the SUPL server hostname needs to be configured using CONFIG_GNSS_SAMPLE_SUPL_HOSTNAME.

To build the sample, run the following command in the sample directory:

`west build -b nrf9160dk_nrf9160_ns`

To program the nRF9160 development kit:

`west flash`


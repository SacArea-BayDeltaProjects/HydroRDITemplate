# RDITemplate
## [CR1000 and CR6 Datalogger template for RDI ADCPs]

*This program is designed for use with the updated station electronics board only.*

USE THE SCRIPT FILES FOR THE ADCPs THAT ARE IN THIS FOLDER FOR THIS PROGRAM ONLY!

Update datalogger operating system to most current version. UPDATING OPERATING SYSTEM WIPES ALL MEMORY FROM DATALOGGER! DO NOT ATTEMPT REMOTE OS UPDATE UNLESS YOU'VE DISCUSSED WITH TV.


If this is a PROGRAM UPDATE to an existing station with new electronics board, verify whether parameters need updating or not. If parameters (startbin/endbin/offsets/ratings, etc). need to be updated, you must DELETE THE FILE IN THE USR DRIVE OF THE DATALOGGER. Otherwise, previous settings will be loaded (This is used if there is an outage at a station it will reboot with most recent parameters).

Open "RDITemplateMaster.dld" with CRBasic Editor.

Before Compiling/Sending to Datalogger:

1. Use "Tools" menu to set .dld datalogger type for the target datalogger (CR6 or CR1000). (Tools>Set .DLD Datalogger Type... OR Ctrl+E)
2. Select proper constants in Customize Constants menu. (Tools>Customize Constants...) This can be done manually if the Customize Constants function isn't working well.

   Description of constants:
  
    **WqSonde:** If sonde is present, select which communication type is being used and whether "Burst" sampling should occur.

    **DWRSonde:** Is the sonde maintained by DWR? If so, this forces the sonde time update to happen to PST rather than to UTC.

    **TopBottom:** Are there two sondes daisy-chained together? If so this will duplicate all serial communications for water quality (parameter checks, set delimiters, get 	instant data points, wipe, and collect bursts) for the second sonde and assign the values with a BTM prefix.
    
    **RedSonde:** Is there a second sonde running through a second SOA? This will NOT run burst data! Only for single point collection currently.
    
    **HasIridium:** Is there an Iridium telemetry system as a backup to the modem?
    
    **HasSuna:** Is there a SUNA in the system? If so, this will perform SUNA operations and create and parse datatables for the SUNA.
    
    **WqBaud:** If sonde is present, select baud rate for SERIAL communications. Most likely, 38400 is correct.
    
    **VmBaud:** Select baud rate for velocity meter. Typical setup: 38400.
    
    **IsWqMax232:** Is there a Max232 chip present in the communication system for the sonde? <ONLY FOR SERIAL COMMUNICATION
      *Max232 board is necessary for sondes on CR1000 C-ports and CR6 U-ports*
    
    **MaxBurstCnt:** If WqSonde = WQ_SERIAL_BURST select the number of individual samples taken during each burst up to a maximum of 30. Each sample gives an instantaneous value for all parameters with one second between samples. 30 samples (taking 30 seconds) should be used unless otherwise discussed. 
    
    **IsVmMax232:** Is there a Max232 chip present in the communication system for the velocity meter?
      *Not necessary on either set of ports, but should probably be used for CR6 U-ports or long RS232 cable runs.*
    
    **IsSpecStudy:** Do we need the "VelData" table active? (This table contains more velocity data than our normal tables)

    **HasGPS:** Is there a real-time GNSS receiver at the station. (This will only compile on CR6)
    
    **GetNoiseFloor:** Do we need to collect "Noise Floor" data for the station for sediment surrogate models?
    
    **HasSlave:** Do we want to send data to a slave datalogger. Mostly Freeport specific. 
      *THIS DOES NOT CONFIGURE MASTER/SLAVE COMMUNICATIONS, ONLY FOR SENDING SPECIFIC DATA TO THE SLAVE.*
    
    **HasVM:** Is there a velocity meter present?
    
    **UseAutoRange:** Is this an uplooking ADCP that we want bin distribution to scale based on water level above the instrument?
    
    **WQDeadCntMax:** How many minutes should the datalogger wait to cycle power to the sonde if no data are coming in? Intervals of 5min.
    
    **VMDeadCntMax:** How many minutes should the datalogger wait to cycle power to the ADCP if not data are coming in? Intervals of 5min.
    
    **StoreAllToCard:** Should we store 1min spike data (RSSI, Vel, Corr) to memory card?
      *This does not affect normal operation of memory card*
    
    **Debugging:** Do we want to see debugging variables in the Public Table?
    
    **SeeAllVel:** Do we want to see all raw velocity data in the Public Table? (V1, V2, V3, V4, VMBinVel)
      *This does not affect collection of any data*
    
    **SeeRawWQ:** Do we want to see all raw water quality data in the Public Table? (t(), WqData(),WqIn (Raw String), WqDelay, RecBurstFlag)
      *This does not affect collection of any data*
      
    **ReadExtPwr:** Do we need to read an external power source using a 10:1 converter? (Ex. WorkHorse 12-48V converter)
      
  **FOR FURTHER CLARIFICATION ON THESE, PLEASE TALK TO TV.**

3. Conditional Compile and Save. (Compile>Conditional Compile and Save) Rename with target station ID and date.

4. In new file, define which ports velocity meter and sonde will communicate through. (Scroll to top of program, Ctrl+F for "AdcpPort" to find location to define these parameters. IMPORTANT: If using CR6, ports ComC1 or ComC3, use DevConfig to configure these ports to RS232 (NOT TTL or other).
  *On CR1000, Port options are: Com1, Com2. On CR6: ComC1, ComC3, ComU1. Other ports are currently reserved for SDI12 instruments.*

5. Define station specific parameters. (GoTo>Navigation>Subroutine Section. "GetStartup" is the name of the proper subroutine for these changes). (If sidelooker mounted on right bank, FlowSign = 1. If sidelooker mounted on left bank, FlowSign = -1). Available options for SelStgPrime, SelStgSecond are: UpBeam, VMPress, PSPress, PSPress2, and WqPress.

6. Save and Compile program, look for compilation errors (Hopefully none!). (Compile>Save and Compile)

7. If none, load file onto datalogger.


*Some Advanced use information:*

- **PosFlowDir** can only be used on 4-beam uplookers using **EARTH** coordinate transformation. MUST CALIBRATE ADCP COMPASS
  - It's use will be disabled in any other configuration after the first ensemble is collected.
- If using **BEAM** coordinate transformation on a ChannelMaster, you MUST obtain an external heading as there is NO INTERNAL COMPASS.
- If using a **Bi-Directional Rating** (Ebb rating/Distribution is different than Flood rating/distribution):
  - Must use a sidelooking ADCP with INSTRUMENT coordinates.
  - Set "BiRating" to True in Startup Parameters. 
  - Set StartBin, EndBin, StartEbbBin, EndEbbBin, StartFldBin, EndFldBin to appropriate values. *StartBin and EndBin should include ALL     BINS used for both ebb and flood calculations*.
  - Set BiRatTran (IndexVel transition point to switch between ratings).
- **Y-velocity** can be included in the IndexVel calculations by setting **UseV2 = True**.
- If using uplooker and **AutoRange** function, the variable **CutOffBins** defines how many bins to cut from the Index Calculation to account for interference from the surface (wind waves, for example).

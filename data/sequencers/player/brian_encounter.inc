###################
####  Bookie  #####
###################

Move Bookie_Base
	Requires	ENCOUNTER CLIPBOARD OBSERVE PROP HIGH
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Bookie_Base
	CycleMove       Bookie_Base
	CycleMove       Bookie_Left
	CycleMove       Bookie_Right

	Type		male
	  Anim          male\Bookie_Base 1
	  PlayFx        GENERIC\FX_clipboard.FX  0 Constant
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 38
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 88
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 97
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 153
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 162
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 210
	TEnd
MEnd

Move Bookie_Left
	Requires	ENCOUNTER CLIPBOARD OBSERVE PROP HIGH
	Priority        5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Bookie_Base
	CycleMove       Bookie_Base
	CycleMove       Bookie_Left
	CycleMove       Bookie_Right

	Type		male
	  Anim          male\Bookie_Left 1
	  PlayFx        GENERIC\FX_clipboard.FX  0 Constant
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 100
	TEnd
MEnd

Move Bookie_Right
	Requires	ENCOUNTER CLIPBOARD OBSERVE PROP HIGH
	Priority        5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Bookie_Base
	CycleMove       Bookie_Base
	CycleMove       Bookie_Left
	CycleMove       Bookie_Right

	Type		male
	  Anim          male\Bookie_Right 1
	  PlayFx        GENERIC\FX_clipboard.FX  0 Constant
	  PlayFX		GENERIC\FX_clipboardShort_SFX.fx 95
	TEnd
MEnd


###################
#####  Vendor #####
###################

Move Vendor_YellRight
	Requires	ENCOUNTER OBSERVE LOITER BUSY
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Vendor_Point
	CycleMove       Vendor_Sweep

	Type		male
	  Anim          male\VendorNew_YellRight 1
	  PlayFX	Generic\VendorCart.fx 0 Constant
	TEnd
MEnd

Move Vendor_Point
	Requires	ENCOUNTER OBSERVE LOITER BUSY
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Vendor_YellRight
	CycleMove       Vendor_Sweep

	Type		male
	  Anim          male\VendorNew_Point 1
	  PlayFX	Generic\VendorCart.fx 0 Constant
	TEnd
MEnd

Move Vendor_Sweep
	Requires	ENCOUNTER OBSERVE LOITER BUSY
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       Vendor_Point
	CycleMove       Vendor_YellRight

	Type		male
	  Anim          male\VendorNew_Sweep 1
	  PlayFX	Generic\VendorCart.fx 0 Constant
	TEnd
MEnd

############################
###### Oil Blaze Toss ######
############################

Move OilBlazeIdle_1
	Requires	ENCOUNTER PROP VANDALIZE LOOKOUT
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove	OilBlazeToss_1
	Sets            CANTMOVE

	Type		male
	  Anim          male\OilBlaze_Base 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

Move OilBlazeToss_1
	Include sequencers\Members\Encounters_Cycles.inc
	Interpolate	2
	Sets            CANTMOVE

	Type		male
	  Anim          male\OilBlaze_Toss 1 115
	  PlayFX	Explosions\medium\Enc_Expolsion_med.fx 104
	  PlayFX	WORLD\Ruins\Enc_MediumFireFast.fx 104
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

#############################################################

Move OilBlazeIdle_2
	Requires	ENCOUNTER PROP VANDALIZE LOOKOUT
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove	OilBlazeToss_2
	Sets            CANTMOVE

	Type		male
	  Anim          male\OilBlaze_Base 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

Move OilBlazeToss_2
	Include sequencers\Members\Encounters_Cycles.inc
	Interpolate	8
	Sets            CANTMOVE

	Type		male
	  Anim          male\OilBlaze_Toss 45 115
	  PlayFX	Explosions\medium\Enc_Expolsion_med.fx 59
	  PlayFX	WORLD\Ruins\Enc_MediumFireFast.fx 59
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

#############################################################

Move OilBlazeIdle_3
	Requires	ENCOUNTER PROP VANDALIZE LOOKOUT
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	NextMove	OilBlazeToss_3
	Sets            CANTMOVE

	Type		male
	  Anim          male\OilBlaze_Base 1
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

Move OilBlazeToss_3
	Include sequencers\Members\Encounters_Cycles.inc
	Interpolate	8
	Sets            CANTMOVE

	Type		male
	  Anim          male\OilBlaze_Toss 70 115
	  PlayFX	Explosions\medium\Enc_Expolsion_med.fx 34
	  PlayFX	WORLD\Ruins\Enc_MediumFireFast.fx 34
	  PlayFX	Generic\Torch_sm.fx 0 Constant
	TEnd
MEnd

############################
###### Soul Take Idle ######
############################

Move SoulTake_Rip
	Requires	ENCOUNTER TRAIN LOW IDLE
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	Interpolate	8
	CycleMove       SoulTake_Rip
       	CycleMove       SoulTake_Alt


	Type		male
	  Anim          male\SoulTake_Rip 1
	  PlayFX	GENERIC\SoulTake.fx 0 Constant
	TEnd
MEnd

Move SoulTake_Alt
	Requires	ENCOUNTER TRAIN LOW IDLE
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	Interpolate	8
	CycleMove       SoulTake_Rip
       	CycleMove       SoulTake_Alt


	Type		male
	  Anim          male\SoulTake_Alt 1
	  PlayFX	GENERIC\SoulTake.fx 0 Constant
	TEnd
MEnd

############################
#######   Manacles   #######
############################

Move Manacles_Pose1
	Requires	ENCOUNTER PROP SLEEP WAIT LOW
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\Manacles_Pose1 1
	TEnd
MEnd

#############################################################

Move Manacles_Pose2
	Requires	ENCOUNTER PROP SLEEP WAIT LOW
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\Manacles_Pose2 1
	TEnd
MEnd

#############################################################

Move Manacles_Pose3
	Requires	ENCOUNTER PROP SLEEP WAIT LOW
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\Manacles_Pose3 1
	TEnd
MEnd

#############################################################
## Jackhammer - Vertical
#############################################################

Move Jackhammer_Vertical
	Requires	ENCOUNTER PROP BUSY WORK LOW
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Jackhammer_Vertical
	CycleMove       Jackhammer_Vertical
	CycleMove       Jackhammer_Wipe

	Type		male
	  Anim          male\JackHammer_Vertical 1
	  PlayFX	Generic\JackHammer.fx 0 Constant
	  PlayFX	Generic\JackHammer_audio.fx 0 Constant
	TEnd
MEnd

Move Jackhammer_Wipe
	Requires	ENCOUNTER PROP BUSY WORK LOW
	Priority        5
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Jackhammer_Vertical

	Type		male
	  Anim          male\JackHammer_Wipe 1
	  PlayFX	Generic\JackHammer.fx 0 Constant
	  PlayFX	Generic\JackHammer_audio2.fx 0 Constant
	TEnd
MEnd

#############################################################
## Jackhammer - Horizontal
#############################################################

Move Jackhammer_Horizontal
	Requires	ENCOUNTER PROP BUSY WORK HIGH
	Priority        5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale ReqInputs

	Type		male
	  Anim          male\JackHammer_Horizontal 1
	  PlayFX	Generic\JackHammer.fx 0 Constant
	  PlayFX	Generic\JackHammer_audio.fx 0 Constant
	TEnd
MEnd

#############################################################
## FLASHLIGHT
#############################################################

Move Flashlight
	Requires	ENCOUNTER FLASHLIGHT OBSERVE
	Priority	3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle

	Type		male
	  Anim          male\flashlight 1
	  PlayFx        GENERIC\FLASHLIGHT.FX  0 Constant
	TEnd
MEnd

#############################################################

Move Flashlight_Up
	Requires	ENCOUNTER FLASHLIGHT HIGH
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Sets            CANTMOVE

	Type		male
	  Anim          male\Flashlight_Up 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

#############################################################

Move Flashlight_Down
	Requires	ENCOUNTER FLASHLIGHT LOW
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle
	Sets            CANTMOVE

	Type		male
	  Anim          male\Flashlight_Down 1
	  PlayFX	Generic\Flashlight_NoSpot.fx 0 Constant
	TEnd
MEnd

##################################################
#### Emote Laptop typing, deleting, and rest #####
##################################################

Move PL_LaptopType
	Requires        ENCOUNTER TYPING
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove       PL_LaptopType
	CycleMove       PL_LaptopType
	CycleMove       PL_LaptopRest
	CycleMove       PL_LaptopDelete
	Interpolate	10

	Type            male
	  Anim          male\emote_LaptopType 1
	  PlayFX		NPCFX\NPC_Typing.fx 1 CONSTANT
	TEnd
MEnd

Move PL_LaptopRest
	Requires        ENCOUNTER TYPING
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove       PL_LaptopType
	CycleMove       PL_LaptopType
	CycleMove       PL_LaptopDelete
	Interpolate	0

	Type            male
	  Anim          male\emote_LaptopRest 1
	  PlayFX		NPCFX\NPC_Typing.fx 90 CONSTANT
	TEnd
MEnd

Move PL_LaptopDelete
	Requires        ENCOUNTER TYPING
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	CycleMove       PL_LaptopType
	CycleMove       PL_LaptopType
	CycleMove       PL_LaptopRest
	Interpolate	0

	Type            male
	  Anim          male\emote_LaptopDelete 1
	  PlayFX		NPCFX\NPC_Typing_Delete.fx 55
	TEnd
MEnd

#########################################################
#################   Portable Laptop    ##################
#########################################################

Move Portable_Laptop
	Requires	ENCOUNTER PROP BUSY ROOTED
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	Interpolate	0
	CycleMove       Portable_Laptop
	CycleMove       Portable_Laptop
	CycleMove       Portable_Laptop_Rest
	CycleMove       Portable_Laptop_Delete

	Type		male
	  Anim          male\Portable_Laptop 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\Portable_Laptop 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

Move Portable_Laptop_Rest
	Requires	ENCOUNTER PROP BUSY ROOTED
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	Interpolate	0
	CycleMove       Portable_Laptop
	CycleMove       Portable_Laptop_Rest

	Type		male
	  Anim          male\Portable_Laptop_Rest 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\Portable_Laptop_Rest 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

Move Portable_Laptop_Delete
	Requires	ENCOUNTER PROP BUSY ROOTED
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY NoSizeScale
	Interpolate	0
	CycleMove       Portable_Laptop
	CycleMove       Portable_Laptop_Rest
	CycleMove       Portable_Laptop_Rest

	Type		male
	  Anim          male\Portable_Laptop_Delete 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\Portable_Laptop_Delete 1
	  PlayFX	Generic\Stand_LaptopTable.fx 0 Constant
	TEnd
MEnd

#############################################################
## ASSUME POSITION
#############################################################

Move Assume_Position_1
	Requires	ENCOUNTER WAIT BUSY ROOTED
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\AssumePosition_1 1
	TEnd
MEnd

Move Assume_Position_2
	Requires	ENCOUNTER WAIT BUSY ROOTED
	Priority	4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\AssumePosition_2 1
	TEnd
MEnd

Move Assume_Position_3
	Requires	ENCOUNTER WAIT BUSY ROOTED
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\AssumePosition_3 1
	TEnd
MEnd

Move Assume_Position_4
	Requires	ENCOUNTER WAIT BUSY ROOTED
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\AssumePosition_4 1
	TEnd
MEnd

######################
####  PL_PeerIn  #####
######################

Move PeerIn_Start
	Requires	ENCOUNTER OBSERVE WALL MID
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Cyclemove	PeerIn_Cycle

	Type		male
	  Anim          male\PeerIn 1 20
	TEnd
MEnd

Move PeerIn_Cycle
	Requires	ENCOUNTER OBSERVE WALL MID
	Priority        4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Cyclemove	PeerIn_LookQuick
	Cyclemove	PeerIn_Cycle
	Cyclemove	PeerIn_Cycle

	Type		male
	  Anim          male\PeerIn 20
	TEnd
MEnd

Move PeerIn_LookQuick
	Requires	ENCOUNTER OBSERVE WALL MID
	Priority        4
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Cyclemove	PeerIn_Start

	Type		male
	  Anim          male\lookquick 1
	TEnd
MEnd

#############################################################
## Clockwork Siphon
#############################################################

Move ClockworkSiphon
	Requires	ENCOUNTER DRAIN SPELLCAST BUSY MID
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\ClockworkSiphon 1
	  Playfx	Powers\ElectricityControl\Clockwork_Siphon.fx  0 Constant
	TEnd
MEnd

#############################################################
## Clockwork Siphon - Down
#############################################################

Move ClockworkSiphon_Low
	Requires	ENCOUNTER DRAIN SPELLCAST BUSY LOW
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\ClockworkSiphon_Down 1
	  Playfx	Powers\ElectricityControl\Clockwork_Siphon_alt.fx  0 Constant
	TEnd
MEnd

#############################################################
## Clockwork Siphon - Up
#############################################################

Move ClockworkSiphon_High
	Requires	 ENCOUNTER DRAIN SPELLCAST BUSY HIGH
	Priority	5
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\ClockworkSiphon_Up 1
	  Playfx	Powers\ElectricityControl\Clockwork_Siphon_alt.fx  0 Constant
	TEnd
MEnd

#############################################################
## Pamphlet
#############################################################

Move PL_Pamphlet
	Requires	ENCOUNTER PROP LOITER BECKON
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\Pamphlet 1
	  Playfx	GENERIC\FX_Pamphlet.fx 0 Constant
	TEnd
MEnd

#############################################################
## Big Pry Bar
#############################################################

Move PL_PryBarBig
	Requires	ENCOUNTER PROP WORK VANDAL
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Idle.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\PryBarBig 1
	  Playfx	GENERIC\FX_PryBarBig.fx 0 Constant
	TEnd
MEnd


#############################################################
## ThreeCardMonty
#############################################################

Move PL_ThreeCardMonty_Idle
	Requires	ENCOUNTER THREECARDMONTY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	CycleMove       PL_ThreeCardMonty_Start
	Scale		0.35

	Type		male
	  Anim		male\ThreeCardMonty_Loop 1
	  Playfx	GENERIC\FX_CollapseTable_Sit.fx  0 Constant
	TEnd
MEnd

Move PL_ThreeCardMonty_Start
	Requires	ENCOUNTER THREECARDMONTY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PL_ThreeCardMonty_Loop
	scale		1.5

	Type		male
	  Anim		male\ThreeCardMonty 1 20
	  Playfx	GENERIC\FX_CollapseTable.fx  0 Constant
	  PlayFX	NPCFX\NPC_ClothShort_SFX.fx 1

	TEnd
MEnd

Move PL_ThreeCardMonty_Loop
	Requires	ENCOUNTER THREECARDMONTY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       PL_ThreeCardMonty_End
	Scale		1.5

	Type		male
	  Anim		male\ThreeCardMonty 20 136
	  Playfx	GENERIC\FX_CollapseTable.fx  0 Constant
	  PlayFX	NPCFX\NPC_CupDown_SFX.fx 1
	  PlayFX	NPCFX\NPC_CupMove_SFX.fx 10
	  PlayFX	NPCFX\NPC_CupUp_SFX.fx 18
	  PlayFX	NPCFX\NPC_CupDown_SFX.fx 20
	  PlayFX	NPCFX\NPC_CupMove_SFX.fx 30
	  PlayFX	NPCFX\NPC_CupUp_SFX.fx 38
	  PlayFX	NPCFX\NPC_CupDown_SFX.fx 40
	  PlayFX	NPCFX\NPC_CupMove_SFX.fx 50
	  PlayFX	NPCFX\NPC_CupUp_SFX.fx 58
	  PlayFX	NPCFX\NPC_CupDown_SFX.fx 60
	  PlayFX	NPCFX\NPC_CupMove_SFX.fx 70
	  PlayFX	NPCFX\NPC_CupUp_SFX.fx 78
	  PlayFX	NPCFX\NPC_CupDown_SFX.fx 80
	  PlayFX	NPCFX\NPC_CupMove_SFX.fx 90
	  PlayFX	NPCFX\NPC_CupUp_SFX.fx 98
	  PlayFX	NPCFX\NPC_CupMove_SFX.fx 100
	  PlayFX	NPCFX\NPC_CupUp_SFX.fx 110
	TEnd
MEnd

Move PL_ThreeCardMonty_End
	Requires	ENCOUNTER THREECARDMONTY
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	PL_ThreeCardMonty_Idle
	Scale		1.5

	Type		male
	  Anim		male\ThreeCardMonty 136
	  Playfx	GENERIC\FX_CollapseTable.fx  0 Constant
	  PlayFX	NPCFX\NPC_ClothLong_SFX.fx 5
	TEnd
MEnd

################################################################################################
##################################    PL_GuardNodOff    ########################################

Move PL_GuardNodOff_Alert
	Requires	ENCOUNTER PROP SLEEP SIT
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.5
	CycleMove	PL_GuardNodOff_In
	CycleMove	PL_GuardNodOff_Alert
	CycleMove	PL_GuardNodOff_Alert

	Type		male
	  Anim          male\GuardNodOff_Idle 1
	  PlayFx        Weapons\Shotgun.fx 0 Constant
	TEnd
MEnd

Move PL_GuardNodOff_In
	Requires	ENCOUNTER PROP SLEEP SIT
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.5
	CycleMove	PL_GuardNodOff_Loop

	Type		male
	  Anim          male\GuardNodOff 1 123
	  PlayFx        Weapons\Shotgun.fx 0 Constant
	TEnd
MEnd

Move PL_GuardNodOff_Loop
	Requires	ENCOUNTER PROP SLEEP SIT
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.3
	CycleMove	PL_GuardNodOff_Out
	CycleMove	PL_GuardNodOff_Loop
	CycleMove	PL_GuardNodOff_Loop

	Type		male
	  Anim          male\GuardNodOff 124 189
	  PlayFx        Weapons\Shotgun.fx 0 Constant
	TEnd
MEnd

Move PL_GuardNodOff_Out
	Requires	ENCOUNTER PROP SLEEP SIT
	Priority        4
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	Scale           0.5
	CycleMove	PL_GuardNodOff_Alert

	Type		male
	  Anim          male\GuardNodOff 190
	  PlayFx        Weapons\Shotgun.fx 0 Constant
	TEnd
MEnd

#############################################################
## TOUCH SCREEN
#############################################################

Move TouchScreen
	Requires	ENCOUNTER TOUCHSCREEN
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       ScreenIdleLoop
	CycleMove       ScreenIdleLoop
	CycleMove       TouchScreenHit

	Type		male
	  Anim          male\emote_TouchScreen 1 117
	TEnd
MEnd

Move ScreenIdleLoop
	Requires	ENCOUNTER TOUCHSCREEN
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       TouchScreenHit
	CycleMove       TouchScreen

	Type		male
	  Anim          male\emote_TouchScreen 110
	TEnd
MEnd

Move TouchScreenHit
	Requires	ENCOUNTER TOUCHSCREEN
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PickRandomly
	CycleMove       ScreenIdleLoop
	CycleMove       TouchScreen

	Type		male
	  Anim          male\emote_TouchScreenHit 1
	TEnd
MEnd

###################################################
#################   PL_OutCold   ##################
###################################################

Move OutCold01
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 1 3
	TEnd

	Type		fem
	  Anim          fem\PL_OutCold 1 3
	TEnd
MEnd

Move OutCold02
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 5 8
	TEnd

	Type		fem
	  Anim          fem\PL_OutCold 5 8
	TEnd
MEnd

Move OutCold03
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 10 13
	TEnd

	Type		fem
	  Anim          fem\PL_OutCold 10 13
	TEnd
MEnd

Move OutCold04
	Requires	ENCOUNTER OUTCOLD
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle PICKRANDOMLY
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_OutCold 15 18
	TEnd

	Type		fem
	  Anim          fem\PL_OutCold 15 18
	TEnd
MEnd

###################################################
#################   PL_OutCold  CUTSCENE ONLY  ##################
###################################################

Move OutCold01_Cutscene
	Requires	ENCOUNTER OUTCOLD JUSTFUCKINDOIT
	Priority        3
	Include sequencers\Interrupts\Encounters_Scripted.inc
	Flags		NoSizeScale NotSelectable Cycle
	Sets            CANTMOVE
	sticksonchild	ENCOUNTER OUTCOLD JUSTFUCKINDOIT

	Type		male
	  Anim          male\PL_OutCold 1 3
	TEnd

	Type		fem
	  Anim          fem\PL_OutCold 1 3
	TEnd
MEnd

#############################################################
## DRINK
#############################################################

Move Drinker1_Idle
	Requires	ENCOUNTER DRINK ONE
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Drinker1_Idle
	CycleMove       Drinker1_Idle
	CycleMove       Drinker1_SipBig
	CycleMove       Drinker1_Sip
	Scale		0.75

	Type		male
	  Anim          male\PL_DrinkIdle 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 Constant
	TEnd
MEnd

Move Drinker1_Sip
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Drinker1_Idle

	Type		male
	  Anim          male\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 Constant
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 1
	TEnd
MEnd

Move Drinker1_SipBig
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Drinker1_Idle

	Type		male
	  Anim          male\PL_DrinkSipBig 1
	  PlayFx        Generic\FX_DrinkingCup.fx 0 Constant
	  PlayFX		NPCFX\NPC_Drink_SFX.fx 1
	TEnd
MEnd

#############################################################
## DRINK - CLAY MUG
#############################################################

Move Drinker2_Idle
	Requires	ENCOUNTER DRINK TWO
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Drinker2_Idle
	CycleMove       Drinker2_Idle
	CycleMove       Drinker2_SipBig
	CycleMove       Drinker2_Sip
	Scale		0.75

	Type		male
	  Anim          male\PL_DrinkIdle 1
	  PlayFx        Generic\FX_DrinkingClayMug.fx 0 Constant
	TEnd
MEnd

Move Drinker2_Sip
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Drinker2_Idle

	Type		male
	  Anim          male\PL_DrinkSip 1
	  PlayFx        Generic\FX_DrinkingClayMug.fx 0 Constant
	TEnd
MEnd

Move Drinker2_SipBig
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	CycleMove       Drinker2_Idle

	Type		male
	  Anim          male\PL_DrinkSipBig 1
	  PlayFx        Generic\FX_DrinkingClayMug.fx 0 Constant
	TEnd
MEnd

#############################################################
## TALK
#############################################################

Move TalkIdle
	Requires	ENCOUNTER GESTICULATE
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       TalkIdle
	CycleMove       Talk02
	CycleMove       Talk03

	Type		male
	  Anim          male\PL_TalkIdle 1
	TEnd
MEnd

Move Talk01
	Requires	ENCOUNTER GESTICULATE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	Interpolate	15
	CycleMove       TalkIdle
	CycleMove       TalkIdle
	CycleMove       Talk02
	CycleMove       Talk03

	Type		male
	  Anim          male\PL_Talk01 1
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\PL_Talk01 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 68
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\PL_Talk01 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 68
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\PL_Talk01 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 68
	TEnd
MEnd

Move Talk02
	Requires	ENCOUNTER GESTICULATE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	scale		1.2
	CycleMove       TalkIdle
	CycleMove       TalkIdle
	CycleMove       Talk01
	CycleMove       Talk03

	Type		male
	  Anim          male\PL_Talk02 1
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\PL_Talk02 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 40
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\PL_Talk02 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 40
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\PL_Talk02 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoLong.fx 40
	TEnd
MEnd

Move Talk03
	Requires	ENCOUNTER GESTICULATE
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       TalkIdle
	CycleMove       TalkIdle
	CycleMove       Talk01
	CycleMove       Talk02

	Type		male
	  Anim          male\PL_Talk03 1
	TEnd

	Type		PraetorianClockworkMale
	  Anim          male\PL_Talk03 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 35
	TEnd
	Type		PraetorianClockworkFem
	  Anim          male\PL_Talk03 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 35
	TEnd
	Type		PraetorianClockworkHuge
	  Anim          male\PL_Talk03 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 1
	  PlayFX		ENEMYFX\Clockwork\antimatter\Clockwork_Behavior_ServoShort.fx 35
	TEnd
MEnd

#######################
####  PL_Fishing  #####
#######################

Move PL_Fishing
	Requires	ENCOUNTER FISHING
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale

	Type		male
	  Anim          male\PL_Fishing 1
	  Playfx	Generic\FX_FishingRod.fx  0 Constant
	TEnd
MEnd

###########################
####  PL_Fishing_Low  #####
###########################

Move PL_FishingLowIdle
	Requires	ENCOUNTER FISHING LOW
	Priority        3
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove       PL_FishingLowIdle
	CycleMove       PL_FishingLowIdle
	CycleMove       PL_FishingLow

	Type		male
	  Anim          male\PL_FishingLowIdle 1
	  Playfx	Generic\FX_FishingRod.fx  0 Constant
	TEnd
MEnd

Move PL_FishingLow
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale ReqInputs
	NextMove        Ready
	CycleMove       PL_FishingLowIdle
	CycleMove       PL_FishingLowIdle
	CycleMove       PL_FishingLowIdle
	CycleMove       PL_FishingLow

	Type		male
	  Anim          male\PL_FishingLow 1
	  Playfx	Generic\FX_FishingRod.fx  0 Constant
	TEnd
MEnd

###########################
##### Purse tug Copy  #####
######################

Move PurseTug_Pickup
	Requires	ENCOUNTER PURSEPICKUP
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	NextMove        P_PursePickupPush
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\purse_tug 1
	TEnd
MEnd

Move P_PursePickupPush
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        P_PursePickup
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_PursePickUp 1 60
	  Playfx	Generic\FX_ClosedPurse.fx 0 Constant
	TEnd
MEnd

Move P_PursePickup
	Include sequencers\Members\Encounters_Cycles.inc
	NextMove        P_PursePUMug
	Sets            CANTMOVE

	Type		male
	  Anim          male\PL_PursePickUp 61
	  Playfx	Generic\FX_ClosedPurse.fx 0 Constant
	TEnd
MEnd

#############################################################

Move P_PursePUMug
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle
	scale		0.75

	Type		male
	  Anim          male\PL_PurseMug 1 100
	  Playfx	Generic\FX_OpenPurse.fx 0 Constant
	TEnd
MEnd

################################################################################################
##################################    PL_Bane_BombHole   #######################################

Move Bane_Bomb_Pre
	Requires	ENCOUNTER BOMBHOLE
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Bane_Idle

	Type		male
	  Anim          male\GrenadeBombHole 1 52
	  # PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd
MEnd

Move Bane_Idle
	Requires	ENCOUNTER BOMBHOLE
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Interpolate	0
	CycleMove	Bane_Idle
	CycleMove	Bane_Idle
	CycleMove	Bane_Bomb_Post

	Type		male
	  Anim          male\GrenadeBombHoleIdle 1
	TEnd
MEnd

Move Bane_Bomb_Post
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	Interpolate	0
	CycleMove	Bane_Bomb_Pre
	CycleMove	Bane_Activate

	Type		male
	  Anim          male\GrenadeBombHole 55
	  Playfx	V_COV\EnemyPowers\Arachnos\FX_BaneGrenade.fx 0 #Constant
	  PlayFX	NPCFX\NPC_BaneBomb_SFX.fx 30
	TEnd
MEnd

Move Bane_Activate
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	CycleMove	Bane_Bomb_Pre

	Type		male
	  Anim          male\wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd
MEnd

################################################################################################
##################################    PL_Bane_MaceScan   #######################################

Move Bane_MaceScan
	Requires	ENCOUNTER MACESCAN
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	Sets            CANTMOVE
	Scale		.8

	Type		male
	  Anim          male\MaceScan 1
	  PlayFX	V_COV\EnemyPowers\Arachnos\F_Weapon_BaneSpiderMace.fx 0 Constant
	TEnd
MEnd

################################################################################################
##################################    PL_Bane_MaceScan   #######################################

Move Bane_MaceScanDown
	Requires	ENCOUNTER MACESCANDOWN
	Priority	2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle FixArmReg
	Sets            CANTMOVE
	Scale		0.8

	Type		male
	  Anim          male\MaceScanDown 1
	  PlayFX	V_COV\EnemyPowers\Arachnos\F_Weapon_BaneSpiderMace.fx 0 Constant
	TEnd
MEnd

######################################
###### TESTING Hand + Drop Ambush ####
######################################

Move Arachnoid_Hang_Pre
	Requires	ENCOUNTER ARACHSLEEP
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale CantMove
	CycleMove	Arachnoid_Hang
	NextMove	Arachnoid_DropAmbush

	Type		male
	  Anim          male\Arachnoid_HangCeiling 1
	TEnd
MEnd

Move Arachnoid_Hang
	Requires	ENCOUNTER ARACHSLEEP
	Priority        2
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale CantMove
	NextMove	Arachnoid_DropAmbush
	Interpolate	0

	Type		male
	  Anim          male\Arachnoid_HangCeiling 1 80
	TEnd
MEnd

Move Arachnoid_DropAmbush
	Flags		NoSizeScale CantMove #not selectable
	NextMove	Mon_jumpFall
	Interpolate	0

	Type		male
	  Anim          male\Arachnoid_DropAmbush 1
	TEnd
MEnd


#################################
###### Arachnoid Spin COCOON ####
#################################

Move SpinCocoon_Idle_Pre
	Requires	ENCOUNTER SPINCOCOON
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		NoSizeScale
	NextMove	SpinCocoon_Idle

	Type		male
	  Anim		male\Arachnoid_SpinCocoon 1 240
	  PlayFX	V_COV\EnemyPowers\Arachnos\F_ArachnoidCocoon.fx 0 Constant
	TEnd
MEnd

Move SpinCocoon_Idle
	Requires	ENCOUNTER SPINCOCOON
	Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NoSizeScale
	SticksOnChild   SPINCOCOON HIGH

	Type		male
	  Anim		male\Arachnoid_SpinCocoon 1 240
	  PlayFX	V_COV\EnemyPowers\Arachnos\F_ArachnoidCocoon.fx 0 Constant
	TEnd
MEnd


###################################################
##### Emote Laptop typing, deleting, and rest #####
###################################################
#
#Move PL_LaptopType
#	Requires	ENCOUNTER TYPING
#	Priority        2
#	Include sequencers\Interrupts\Encounters_Idle.inc
#	Include sequencers\Members\Encounters_Idle.inc
#	NextMove        PL_LaptopType02
#
#	Type		male
#	  Anim          male\emote_LaptopType 1
#	TEnd
#MEnd
#
#Move PL_LaptopType02
#	Include sequencers\Members\Encounters_Idle.inc
#	Flags		Cycle FixArmReg
#	CycleMove       PL_LaptopRest
#	CycleMove       PL_LaptopRest
#	CycleMove       PL_LaptopType02
#	Interpolate	0
#
#	Type		male
#	  Anim          male\emote_LaptopType 1
#	TEnd
#MEnd
#
#Move PL_LaptopRest
#	Include sequencers\Members\Encounters_Idle.inc
#	Flags		Cycle #FixArmReg
#	CycleMove       PL_LaptopRest
#	CycleMove       PL_LaptopType02
#	CycleMove       PL_LaptopType02
#	CycleMove       PL_LaptopDelete
#	Interpolate	0
#
#	Type		male
#	  Anim          male\emote_LaptopRest 1
#	TEnd
#MEnd
#
#Move PL_LaptopDelete
#	Include sequencers\Members\Encounters_Idle.inc
#	Flags		Cycle #FixArmReg
#	CycleMove       PL_LaptopRest
#	CycleMove       PL_LaptopRest
#	CycleMove       PL_LaptopType02
#	Interpolate	0
#
#	Type		male
#	  Anim          male\emote_LaptopDelete 1
#	TEnd
#MEnd


###########################################
###### Signature Character Teleport  ######
###########################################

Move Signature_Teleport_Pre
	Requires	ENCOUNTER SIGNATUREEXIT
	Priority        2
	Include sequencers\Interrupts\Encounters_Idle.inc
	##Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Signature_Teleport
	Sets            CANTMOVE

	Type		male
	  Anim          male\wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd

	Type		huge
	  Anim          huge\wristband_activate 35 60
	  PlayFX	V_COV\Powers\Robotics\Wristband.fx 0
	TEnd

MEnd

Move Signature_Teleport
	Requires	ENCOUNTER SIGNATUREEXIT
	##Include sequencers\Members\Encounters_Scripted.inc
	NextMove        Signature_Teleport_Post
	Flags		NotSelectable
	Interpolate	15
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\absorb 1
	  PlayFX	V_COV\EnemyPowers\Guards\F_TeleportOut_Signatures.fx 0 Constant
	TEnd

	Type		huge
	  Anim          huge\boostup 1
	  PlayFX	V_COV\EnemyPowers\Guards\F_TeleportOut_Signatures.fx 0 Constant
	TEnd

	Type		fem
	  Anim          fem\boostup 1
	  PlayFX	V_COV\EnemyPowers\Guards\F_TeleportOut_Signatures.fx 0 Constant
	TEnd
MEnd

Move Signature_Teleport_Post
	Requires	ENCOUNTER SIGNATUREEXIT
	##Include sequencers\Members\Encounters_Cycles.inc
	Flags		Cycle NotSelectable
	Sets            CANTMOVE NOCOLLISION

	Type		male
	  Anim          male\Skel_ready2 1
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd


	Type		huge
	  Anim          huge\Skel_ready 1 60
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd

	Type		fem
	  Anim          fem\Skel_ready2 1
	  PlayFx        GENERIC\ENTITYVANISH.FX  0 CONSTANT
	TEnd
MEnd
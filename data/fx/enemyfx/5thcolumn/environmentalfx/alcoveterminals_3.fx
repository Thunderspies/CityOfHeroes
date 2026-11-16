#############################################################
## EMachine.fx
#############################################################

FxInfo
########################################################




Condition
	On	Time
	Time	0

	Event
		EName	AlcoveOffsets
		Type	Start
		At	Root
		geom	FX_EmpowermentAlcoveAltPivots
		Sound AlcoveTerminals3_Loop 60 60 .4
	End

	Event
		Ename	CenterChamber
		Type	Start
		At	AlcoveOffsets
		AltPiv	1
	End

#############################################################
###	CENTER GLOWS 		  ###########################
#############################################################

	Event
		EName	lightning
		Type	Local
		At	AlcoveOffsets
		AltPiv	1
		Part	:Ground_Perimeter_Electricity.part
		Part	:Ground_Perimeter.part
		Part	:Circle_Fill.part
		Sound ChainLight2_loop 80 80 .5
	End

#############################################################
###	TERMINAL GLOWS 		  ###########################
#############################################################

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	2

		Part	:SmallBeamFlare01.part
		Part	:LargeBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:BeamCore_PowerChamber.part
		PMagnet	CenterChamber
	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	3

		Part	:SmallBeamFlare01.part
		Part	:LargeBeamFlare01.part

		Part	:SmallBeamFlare02.part
		Part	:BeamCore_PowerChamber.part
		PMagnet	CenterChamber

	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	4

		Part	:SmallBeamFlare01.part
		Part	:LargeBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:BeamCore_PowerChamber.part
		PMagnet	CenterChamber

	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	5
		Part	:SmallBeamFlare01.part
		Part	:LargeBeamFlare01.part
		Part	:SmallBeamFlare02.part
		Part	:BeamCore_PowerChamber.part
		PMagnet	CenterChamber
	End


#############################################################


End
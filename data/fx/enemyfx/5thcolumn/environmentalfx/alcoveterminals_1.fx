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
		Sound AlcoveTerminals1_Loop 40 40 .4

	End

	Event
		Ename	CenterChamber
		Type	Start
		At	AlcoveOffsets
		AltPiv	1
	End
End


Condition
	On	Cycle
	Time	11




#############################################################
###	TERMINAL GLOWS 		  ###########################
#############################################################


	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	2

		Part	:Embers_Traveling.part
		Part	:SmallBeamFlare_Phase1.part
		Part	:BeamCore_PowerChamber_Phase1.part
		PMagnet	CenterChamber
		Lifespan 10

	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	4

		Part	:Embers_Traveling.part
		Part	:SmallBeamFlare_Phase1.part
		Part	:BeamCore_PowerChamber_Phase1.part
		PMagnet	CenterChamber
		Lifespan 10

	End




#############################################################


End

Condition
	On	Cycle
	Time	8

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	3

		Part	:Embers_Traveling.part
		Part	:SmallBeamFlare_Phase1.part
		Part	:BeamCore_PowerChamber_Phase1.part
		PMagnet	CenterChamber
		Lifespan 5
	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	5

		Part	:Embers_Traveling.part
		Part	:SmallBeamFlare_Phase1.part
		Part	:BeamCore_PowerChamber_Phase1.part
		PMagnet	CenterChamber
		Lifespan 5
	End
End
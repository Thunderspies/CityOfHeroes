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
		Sound AlcoveTerminals4_Loop 70 70 .4
	End

	Event
		Ename	CenterChamber
		Type	Start
		At	AlcoveOffsets
		AltPiv	1
	End

#############################################################
###	CENTER GLOWS		  ###########################
#############################################################

	Event
		EName	lightning
		Type	Local
		At	AlcoveOffsets
		AltPiv	1
		Part	:Ground_Perimeter_Electricity.part
		Part	:Ground_Perimeter.part
		Part	:Circle_Fill.part
		Part4	:ChainLightningArchs1.part
		Part5	:ChainLightningArchs.part
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

		Part	:LargeBeamFlare01.part
		Part	:LargeBeamFlare02.part
		Part	:BeamCore_PowerChamber_Large.part
		Part	ENEMYFX\5thColumn\Portal\Embers_Traveling.part
		Part	:ElectricBeamQuick_Exploding.part
		Part	:ElectricBeamQuick_Large.part
		Part	:GlowQuick_Large.part
		PMagnet	CenterChamber
	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	3

		Part	:LargeBeamFlare01.part
		Part	:LargeBeamFlare02.part
		Part	:BeamCore_PowerChamber_Large.part
		Part	ENEMYFX\5thColumn\Portal\Embers_Traveling.part
		Part	:ElectricBeamQuick_Exploding.part
		Part	:ElectricBeamQuick_Large.part
		Part	:GlowQuick_Large.part
		PMagnet	CenterChamber

	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	4

		Part	:LargeBeamFlare01.part
		Part	:LargeBeamFlare02.part
		Part	:BeamCore_PowerChamber_Large.part
		Part	ENEMYFX\5thColumn\Portal\Embers_Traveling.part
		Part	:ElectricBeamQuick_Exploding.part
		Part	:ElectricBeamQuick_Large.part
		Part	:GlowQuick_Large.part
		PMagnet	CenterChamber

	End

	Event
		Ename	EMOffset4
		Type	Start
		At	AlcoveOffsets
		AltPiv	5
		Part	:LargeBeamFlare01.part
		Part	:LargeBeamFlare02.part
		Part	:BeamCore_PowerChamber_Large.part
		Part	ENEMYFX\5thColumn\Portal\Embers_Traveling.part
		Part	:ElectricBeamQuick_Exploding.part
		Part	:ElectricBeamQuick_Large.part
		Part	:GlowQuick_Large.part
		PMagnet	CenterChamber
	End


#############################################################


End
#
#Condition
#	On	Cycle
#	Time 	10
#
#	Event
#		Type	Start
#		At	AlcoveOffsets
#		AltPiv	3
#
#		Pmagnet	CenterChamber
#		Lifespan 5
#	End
#End
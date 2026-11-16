FxInfo

LifeSpan	70

######################################################################################################
########################################  Emitter Rods  ##############################################
######################################################################################################

Condition
	On	Time
	Time	0

	Event	
		EName	Rod_01
		Type	Local
		At	Origin

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_Rod01.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	Rod_02
		Type	Local
		At	Origin
		bhvrOverride
			behavior
			PositionOffset		0 -.6 0
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_Rod01.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	Rod_03
		Type	Local
		At	Origin
		bhvrOverride
			behavior
			PositionOffset		-.6 0 0
			PyrRotate		0 0 90

		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_Rod01.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	Rod_04
		Type	Local
		At	Origin
		bhvrOverride
			behavior
			PositionOffset		0 .6 0
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_Rod01.bhvr
#		Geom	Testing_TargetB
	End

	Event	
		EName	Barrel
		Type	Local
		At	Origin
		bhvrOverride
			behavior
			PositionOffset		.05 .05 2.4
			Scale			1 1 1
		End

		bhvr	V_COV\BaseFx\turrets\Tech\Energy_B_Rod01.bhvr
#		Geom	Testing_TargetB
	End


End




######################################################################################################
########################################  charge Rods  ###############################################
######################################################################################################

Condition
	On 	Time
	Time 	1
	
	Event
		EName 	RodGlow_01
		Type	local
		At	Rod_01
		Part1	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubbles.part	
		Part2	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubblesWhite.part
		Lifespan 25
	End

	Event
		EName 	RodGlow_02
		Type	local
		At	Rod_02
		Part1	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubbles.part
		Part2	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubblesWhite.part
		Lifespan 25
	End

	Event
		EName 	RodGlow_03
		Type	local
		At	Rod_03
		Part1	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubbles.part
		Part2	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubblesWhite.part
		Lifespan 25
	End

	Event
		EName 	RodGlow_04
		Type	local
		At	Rod_04
		Part1	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubbles.part
		Part2	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubblesWhite.part
		Lifespan 25
	End





#	Event
#		EName 	HandGlowLeft1
#		Type	local
#		At	WepL
#		Part1	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubbles.part
#		Part2	V_COV\BaseFx\turrets\Tech\Energy_P_ChargeBubblesWhite.part
#		PMagnet	WepL
#		Lifespan 19
#	End	
End

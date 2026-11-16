#############################################################
## StormCloud_Trail.fx
#############################################################

FxInfo

#########################################################

Condition

	On 	Time
	Time	0


	Event
		ENAME	Cloud
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.1 1.5
		End
		part	:Smoke_Trail.part
		part	:Electricity_Head_Core.part
		part	:Electricity_Head_Bolts.part
		part	:Electricity_Head_Ground.part

	End

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.05 -0.5
		End
		part	:Electricity_Trail.part
		part	:Electricity_Trail_Ground.part
		part	:Electricity_Trail_Ground_Bolts.part
	End

	Event
		ENAME	Cloud
		Type	Local
		At	Root
		part	:Electricity_Stationary_Bolts.part
		part	:Smoke_Stationary.part
		part	:Electricity_Stationary_Head_Bolts.part
		part	:Electricity_Stationary_Ground_Bolts.part
		part	:Electricity_Stationary_Head_Core.part
		part	:Electricity_Stationary_Head_Core_Up.part
	End
End

Condition
	On	Cycle
	Time	10
	Random 	1
	Chance 	0.5

	Event
		ENAME	LeftLLegGlow
		Type	Local
		At	FootL
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Leg_Bolts.part
		part	:Leg_Bolts2.part
		POther  Llegl
		Lifespan 10
	End

	Event
		ENAME	RightLLegGlow
		Type	Local
		At	FootR
		Bhvr	Behaviors\TelportationFade.bhvr
		part	:Leg_Bolts.part
		part	:Leg_Bolts2.part
		POther  LlegR
		Lifespan 10
	End
End

#########################################################

End
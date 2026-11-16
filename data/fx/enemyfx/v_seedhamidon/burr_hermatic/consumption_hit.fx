#############################################################
## Consumption_Hit.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Consumption_Hit_01 100 100 .8
	End

	Event
		Type	local
		At	Chest
		Part	:Disintegration_HitFlash.part
		Part	:Disintegration_Hit.part
		Part	:Disintegration_Hit2.part
		Part	:Disintegration_Hit3.part
		Lifespan 2
	End
	Event
		Type 	Local
		At 	Chest
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:continuing.part
		#Part	:Continuing_Add.part
		Part	:Continuing_Neg.part
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	Local
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
		End
		Part	:Disintegration_Body_Fill_Flat.part
		Part	:Disintegration_Body_Flat.part
	End
End

#############################################################

Condition
	On	Cycle
	Time	15

	Event
		Type	PositOnly
		At	Head
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Disintegration_Body.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Cycle
	Time	12

	Event
		Type	PositOnly
		At	UarmR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Disintegration_Body.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Cycle
	Time	15
	Chance	0.5

	Event
		Type	PositOnly
		At	WepR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Disintegration_Body.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Cycle
	Time	22

	Event
		Type	PositOnly
		At	UarmL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Disintegration_Body.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Cycle
	Time	25
	Chance	0.5

	Event
		Type	PositOnly
		At	WepL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Disintegration_Body.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Cycle
	Time	18
	Chance	0.8

	Event
		Type	PositOnly
		At	UlegR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	1.5 1 0.0
		End
		part	:Disintegration_Foot.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Cycle
	Time	13
	Chance	0.8

	Event
		Type	PositOnly
		At	UlegL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	-1.5 1 0.0
		End
		part	:Disintegration_Foot.part
		Lifespan 	6
		LifespanJitter	4
	End
End

Condition
	On	Cycle
	Time	50
	Chance	0.5

	Event
		Type	PositOnly
		At	FootL
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Disintegration_Foot.part
		Lifespan 	6
		LifespanJitter	4
	End
End



Condition
	On	Cycle
	Time	35
	Chance	0.3

	Event
		Type	PositOnly
		At	FootR
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:Disintegration_Foot.part
		Lifespan 	6
		LifespanJitter	4
	End
End

#############################################################
End
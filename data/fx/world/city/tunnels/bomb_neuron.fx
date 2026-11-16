#############################################################
## Steam_High.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest
		BhvrOverride
			Alpha		255
			Scale		0.45 0.45 0.45
			StartColor	50 50 255
			PyrRotateJitter	0.0 0.0 20.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		-0.1 -.15 0
			SpinJitter	0 .025 0
			EndScale	1.3 1.3 1.3

		End
		Geom	Tintable_EarthGraspHit
		Sound NeutronBomb_Loop 100 100 .5
	End

	Event
		Type	Start
		At	Chest
		BhvrOverride
			Alpha		255
			Scale		0.45 0.45 0.45
			StartColor	50 50 255
			PyrRotateJitter	0.0 0.0 20.0
			FadeInLength	20
			FadeOutLength	20
			Spin 		0.1 .15 0
			SpinJitter	0 .025 0
			EndScale	1.3 1.3 1.3

		End
		Geom	Tintable_EarthGraspHit
	End

	Event
		EName	ChestAnchor
		Type	Start
		At	Chest
		Part	:Aura_Gas.part
		Part	:Aura_Ring.part
		Part	:Aura_Electricity.part
	End

#	Event
#		Type	Start
#		At	Chest
#		BhvrOverride
#			Alpha	50
#			Scale 1.0 1.0 1.0
#		End
#		Geom 	_Anti_Matter_Ball
#	End
End

Condition
	On	Cycle
	Time	10
	Chance	1.0

	Event
		Type	Start
		At	ChestAnchor
		Part	:Aura_Electricity_Zap.part
		LIfespan 5
	End

End

#############################################################

End
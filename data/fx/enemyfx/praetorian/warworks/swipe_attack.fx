#############################################################
## DualBlades - AOE Bridge Attack
#############################################################

FxInfo
LifeSpan	180

#############################################################

Condition
	On 	Time
	Time 	15

	Event
		Type	Local
		At	Mystic
		Sound WWV_Swipe_Attack 150.0 150.0 .8
	End

	Event
		Type	Local
		At	Mystic
		BhvrOverride
			PyrRotate	0.0 0 0.0
		End
		Part	:Blade_Swirl_Fuzzy.part
		Part	:Blade_Swirl_Small.part
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	Mystic
	End

	Event
		Type	Local
		At	Mystic
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	0.0 0 0.0
		End
		Part	:Blade_Swirl_Fuzzy.part
		Part	:Blade_Swirl_Small.part
		Lifespan	1
	End
End


#############################################################

End
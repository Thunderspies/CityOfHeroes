#############################################################
## DualBlades - AOE Bridge Attack
#############################################################

FxInfo
LifeSpan	180

#############################################################


Condition
	On 	Time
	Time 	13

	Event
		Type	Local
		At	Mystic
		Sound WWV_Feint_Attack 150.0 150.0 .8
	End

	Event
		Type	Start
		At	WepR
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			PyrRotate	-45.0 180 0.0
		End
		Part	:Blade_Swirl_Small.part
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	25

	Event
		Type	Local
		At	WepL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 1.0 0.0
			PyrRotate	0.0 180 0.0
		End
		Part	:Blade_Swirl_Small.part
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	2
	End
End


#############################################################

End
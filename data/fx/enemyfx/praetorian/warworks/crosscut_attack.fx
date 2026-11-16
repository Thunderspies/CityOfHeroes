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
		Sound WWV_CrossCut_Attack 150.0 150.0 .8
	End

	Event
		Type	Local
		At	Mystic
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	0.0 90 0.0
		End
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	1
	End
End

#############################################################

Condition
	On 	Time
	Time 	23

	Event
		Type	Start
		At	Mystic
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PyrRotate	0.0 35 0.0
		End
		Part	:Blade_Swirl_Fuzzy.part
		Lifespan	1
	End
End

#############################################################

End
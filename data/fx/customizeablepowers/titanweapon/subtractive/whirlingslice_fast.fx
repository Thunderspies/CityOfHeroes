#############################################################
## WhirlingSlice_Fast.fx
#############################################################

FxInfo
Lifespan 40

######################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	TW_WhirlingSlice_AttackFast_01 100 100 .8
	End
End

Condition
	On 	Time
	Time 	16

	Event
		At Chest
		Type PositOnly
		EName Prime
	End

	Event
		At C_Hips
		Type Local
		Bhvr Behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset 3 0 0
			PyrRotate 180 -60 0
		End
		Part :Swoosh_Trail.part
		Lifespan 3
	End

	Event
		At	Prime
		Type	Local
		Bhvr	:ExpandSwooshRings.bhvr
		Part	:Blade_Swirl.part
		Part	:Blade_Blur.part
		Lifespan	12
	End

End

#############################
End

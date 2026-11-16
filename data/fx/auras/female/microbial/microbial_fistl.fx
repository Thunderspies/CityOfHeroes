#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	HandL
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Part	:Microbe_PartGlobs.part
	End

	Event
		Type	Local
		At	HandL
		Part	:BodyGlow.part
	End
End

Condition
	On 	Cycle
	Time 	15

	Event
		Type	Local
		At	HandL
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		ChildFX	:CHILD_Microbial_SmallArea.fx
		Lifespan 120
	End
End

#############################################################

End
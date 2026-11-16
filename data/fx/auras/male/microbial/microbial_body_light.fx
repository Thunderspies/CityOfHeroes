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
		At	Root
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 4 0.0
		End
		Part	:Microbe_Globs_Light.part
	End
End

Condition
	On 	Cycle
	Time 	45

	Event
		Type	Local
		At	Chest
		ChildFX	:CHILD_Microbial_Body.fx
		Lifespan 120
	End
End

#########

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:BodyGlow.part
	End

	Event
		Type	Local
		At	Head
		Part	:BodyGlow.part
	End

	Event
		Type	Local
		At	Hips
		Part	:BodyGlow.part
	End

End


#############################################################

End
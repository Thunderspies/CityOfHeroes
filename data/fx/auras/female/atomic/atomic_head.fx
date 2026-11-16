#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		Part	:Nucleus_Glow_Head.part
	End

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	1
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:Eye_Glow.part
		Part	:Nucleus_Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		Bhvr	behaviors/GenericParticleFade.bhvr
		BhvrOverride
			PositionOffset	0.0 0.01 -0.1
		End
		Part	:Eye_Glow.part
		Part	:Nucleus_Glow_Eyes.part

	End
End

Condition
	On 	Cycle
	Time 	13

	Event
		Type	Local
		At	Chest
		ChildFX	:CHILD_Electron_Head.fx
		Lifespan 40
	End
End

#############################################################

End
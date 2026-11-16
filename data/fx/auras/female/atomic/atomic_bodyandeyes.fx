#############################################################
## Costume Aura - Flies (Body)
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:Nucleus_Glow_Large.part
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
	Time 	10

	Event
		Type	Local
		At	Chest
		ChildFX	:CHILD_Electron.fx
		Lifespan 40
	End
End

#############################################################

End
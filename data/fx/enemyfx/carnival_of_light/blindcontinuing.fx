#############################################################
## Blind - Continuing FX
#############################################################

FxInfo
ClampMaxScale 2.0 2.0 2.0

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	EyeNode
		Type	Local
		At	Hair
		Geom	HairToEyesAdjustment
	End

	Event
		ENAME	ChestGlow
		Type	Local
		At	EyeNode
		AltPiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:BlindGlow01.part
		part	:BlindOrbits01.part
		POther	Neck
	End

	Event
		Type	Local
		At	Root
		ChildFX	IncarnatePowers\Interface\Reactive.fx
	End
End

#############################################################

End
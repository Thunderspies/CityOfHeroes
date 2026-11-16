#########################################################
## Rikti Portal Stabilizer - Large Beam
########################################################

FxInfo

#LifeSpan	90

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	StartOffset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 16 0
		End
	End
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Prong1Offset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	-3.5 9 0
		End
		ChildFX	:RiktiPortalStabilizer_SmallLightningArc1.fx
	End

	Event
		EName	Prong2Offset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	1.5 9 3
		End
		ChildFX	:RiktiPortalStabilizer_SmallLightningArc1.fx
	End

	Event
		EName	Prong3Offset
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	1.75 9 -3
		End
		ChildFX	:RiktiPortalStabilizer_SmallLightningArc1.fx
	End
End

########################################################

End
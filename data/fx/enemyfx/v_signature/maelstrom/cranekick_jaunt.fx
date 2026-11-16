#############################################################
## CraneKick_Jaunt.fx
#############################################################

FxInfo
LifeSpan 100

#############################################################

Condition
	On	Time
	Time	37

	Event
		EName	TargetRootOffset
		Type	Local
		At	T_Root
		BhvrOverride
			PositionOffset	0 0 6
		End
	End
End

Condition
	On	Time
	Time	38

	Event
		EName	TargetLookAt
		Type	Start
		BhvrOverride
			PYRRotate	0 180 0
		End
		At	TargetRootOffset
		Anim	FX_MaelstromJaunt_CraneKick
		Lifespan 34
	End
End

#############################################################

End
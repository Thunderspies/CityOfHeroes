#############################################################
## FX_DemonTail_Demonlings.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		BhvrOverride
			PositionOffset		.01 .105 .065
			PYRRotate		0 0 0
			Scale			0 0 0
		End
		Anim	Demons_Demonling_Tail
	End
End

#############################################################

End
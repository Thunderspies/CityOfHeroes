#############################################################
## FX_DemonTail_Demons.fx
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
			PositionOffset		0.01 0.105 0.065
			PYRRotate		0 0 0
			Scale			0 0 0
		End
		Anim	Demons_DemonLieut_Tail
	End
End

#############################################################

End
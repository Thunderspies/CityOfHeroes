#############################################################
## Male_Shields_Talsorian1.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress IsShield

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LArmL
		BhvrOverride
			PositionOffset		0.0 0.0 0.0
			PyrRotate		0.0 0.0 0.0
		End
		Anim	CustomShield_GloveOffsetNode
	End
End

#############################################################

End
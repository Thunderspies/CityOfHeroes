#############################################################
## Alpha_Body_Male.fx
#############################################################

FxInfo

Flags NotCompatibleWithAnimalHead

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	EyeAnchor
		Type	Local
		At	Eyes
		Geom	CircleOfThorns
	End

	Event
		Type	Local
		At	EyeAnchor
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.08
		End
		AltPiv	1
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.08
		End
		AltPiv	2
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End
End

#############################################################

End
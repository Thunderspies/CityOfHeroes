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
		AltPiv	1
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.08
		End
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

	Event
		Type	Local
		At	EyeAnchor
		AltPiv	2
		BhvrOverride
			PositionOffset	0.0 -0.05 -0.08
		End
		Part	:Star_Eyes.part
		Part	:Glow_Eyes.part
	End

#############################################################

	Event
		EName	Ring
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 0.3 0.0
		End
		part	:LightRay_Up.part
		part	:LightRay_Down.part
		part	:LightRay_Right.part
		part	:LightRay_Left.part

		part	:Star_Body.part
		Part	:Glow_Ring.part
		Part	:Glow_Large_Continuing.part
	End

End

#############################################################

End
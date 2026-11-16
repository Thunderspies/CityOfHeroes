#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 50

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	BubbleAnchor
		Type	Start
		At	Origin
		BhvrOverride
			PositionOffset		0.0 0.25 0.0
			InitialVelocity		0.0 0.05 0.0
			InitialVelocityJitter	.025 .02 .025
		End
	End

	Event

		Type	Local
		At	BubbleAnchor
		BhvrOverride
			PositionOffset	0.0 0.0 0.0
		End
		Part	:Toxic_Bubbles_Small.part
	End
End


Condition
	On	Time
	Time	44

	Event
		Type	Local
		At	BubbleAnchor
		BhvrOverride
			PositionOffset	0.0 0.25 0.0
		End
		Part	:Toxic_Splash_Small.part
		Part	:Toxic_Splash_Droplets_Small.part
	End
End
#############################################################

End
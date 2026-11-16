#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 340

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 1.1 0.0
		End
		Part	:Toxic_Bubbles_Main.part

		Lifespan 20
	End

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.1 0.0
		End
		Part	:Toxic_Ripples.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	18

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Toxic_Splash_Main.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	20

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 0.2 0.0
		End
		Part	:Toxic_Splash_Droplets.part
		Lifespan 20
	End
End
#############################################################

End
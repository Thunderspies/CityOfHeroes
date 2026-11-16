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
			StartJitter	7.5 2.0 7.5
		End
		Part	:Toxic_Splash_Droplets_Initial.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	3

	Event
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	0.0 2.0 0.0
			StartJitter	8.5 2.0 8.5
		End
		Part	:Toxic_Splash_Droplets.part
		Lifespan	1
	End
End

#############################################################

End

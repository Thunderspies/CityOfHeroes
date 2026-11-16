#########################################################
## Vanguard "Redding" Rifle - Thrust (Initial Delayed)
#########################################################

FxInfo

Flags    InheritAlpha
Lifespan	120

## SOUND FX #######################################################

Condition
	On	Time 
	Time	55
	
	Event
		EName	Sound
		Type 	Local
		At	WepR
		Sound throw3 100.0 100.0 .8
	End
End

## BAYONET #######################################################

Condition
	On	Time 
	Time	45
	
	Event
		Ename	Bayonet
		Type	Local
		At	WepR
		Geom	GEO_WepR_VanguardRifle_Bayonet01
		Lifespan	40
	End

	Event
		EName	BayonetGlow
		Type 	Local
		At	WepR
		BhvrOverride
			PositionOffset	3.5 0 0.0
		End
		Part1	:Bayonet\Bayonet_Glow01.part
		Lifespan	50
	End
End

Condition
	On	Time 
	Time	40

	Event
		EName	BayonetGlow
		Type 	Local
		At	WepR
		BhvrOverride
			PositionOffset	3.5 0 0.0
		End
		Part1	:Bayonet\Bayonet_Glow02.part
		Part1	:Bayonet\Bayonet_Glow03.part
	End
End

Condition
	On	Time 
	Time	85

	Event
		EName	BayonetGlow
		Type 	Local
		At	WepR
		BhvrOverride
			PositionOffset	3.5 0 0.0
		End
		Part1	:Bayonet\Bayonet_Glow02.part
		Part1	:Bayonet\Bayonet_Glow03.part
		##Part1	:Bayonet\Bayonet_Sparks01.part
	End
End

#########################################################

End
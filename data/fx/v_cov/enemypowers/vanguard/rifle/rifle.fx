#########################################################
## Vanguard "Redding" Rifle - Rifle FX
#########################################################

FxInfo

Flags    InheritAlpha

## SOUND FX #######################################################

Condition
	On	Time
	Time	9

	Event
		EName	DrawSound
		Type 	Local
		At	WepR
		Sound shotgunout 40 40 .3
	End
End

## RIFLE #######################################################

Condition
	On Time
	Time 9

	Event
		Ename	Rifle
		Type	Local
		At	WepR
		Geom	GEO_WepR_Vanguard_Rifle_02
	End

	Event
		EName	ScopeOffset1
		Type 	Local
		At	Rifle
		Altpiv	2
		Part1	:Scope_Glow01.part
	End

	Event
		EName	ScopeOffset2
		Type 	Local
		At	Rifle
		Altpiv	3
		Part1	:Scope_Glow01.part
	End
End

## BAYONET #######################################################

#Condition
#	On Time
#	Time 9
#
#	Event
#		Ename	Rifle
#		Type	Local
#		At	WepR
#		Geom	GEO_WepR_VanguardRifle_Bayonet01
#	End
#
#	Event
#		EName	BayonetGlow
#		Type 	Local
#		At	WepR
#		BhvrOverride
#			PositionOffset	3.5 0 0.0
#		End
#		Part1	:Bayonet_Glow01.part
#	End
#End

#########################################################

End
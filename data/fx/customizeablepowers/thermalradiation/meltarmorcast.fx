#########################################################
## Thermal Radiation - Melt Armor (Cast)
#########################################################

FxInfo

LifeSpan 70

## SOUND FX #######################################################

Condition
	On 	Time
	Time 	9

	Event
		Type	local
		At	WepR
		Sound atomicblast3 100.0 100.0 1.0
	End
End

Condition
	On 	Time
	Time 	22

	Event
		Type	local
		At	WepR
		Sound Firestreaks 100.0 100.0 1.0
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	local
		At	WepR
		Sound arcoflame 100.0 100.0 .8
	End
End

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepL
		Part	:FireHandz.part
		LifeSpan 70
	End

	Event
		EName	InitialSwordFire
		Type	Local
		At	WepR
		Part	:FireHandz.part
		LifeSpan 70
	End
End

## TARGETTED AOE ###########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	T_Root
		ChildFX	:MeltArmor_AOE.fx
	End
End

#########################################################

End
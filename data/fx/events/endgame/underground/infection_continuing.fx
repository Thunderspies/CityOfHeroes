#############################################################
## LichenHeal_Attack.fx
#############################################################

FxInfo

#############################################################

Input
	InpName	Origin
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Chest
		Part	:Infection_Dust_Continuing.part
	End

	Event
		Type	Local
		At	Root
		Sound 	Infection_Continuing_Loop 100 100 .2
		bhvr	behaviors/Sound/SoundIn05Out10.bhvr
		lifespan 150
	End

End


Condition
	On	Cycle
	Time	10
	Random	1
	Chance	0.5

	Event
		Type	StartPositOnly
		At	UArmL
		Part	:Infection_Spores_Continuing.part
		Lifespan	3
		LifespanJitter	2

	End

	Event
		Type	StartPositOnly
		At	UArmR
		Part	:Infection_Spores_Continuing.part
		Lifespan	3
		LifespanJitter	2

	End


	Event
		Type	StartPositOnly
		At	WepL
		Part	:Infection_Spores_Continuing.part
		Lifespan	3
		LifespanJitter	2

	End

	Event
		Type	StartPositOnly
		At	WepR
		Part	:Infection_Spores_Continuing.part
		Lifespan	3
		LifespanJitter	2

	End
End
#############################################################

End
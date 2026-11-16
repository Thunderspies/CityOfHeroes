#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		sound Plasma_Blast_Hit 200 200 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash.part
		Part	:Hit_Glow_Red.part
		Part	:Hit_Rings_Red.part
		Lifespan 1
	End
End

Condition

	On 	Time
	Time 	2

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash.part
		Part	:Hit_Glow_Red.part
		Part	:Hit_Rings_Red.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	5
	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash.part
		Part	:Hit_Glow_Red.part
		Part	:Hit_Rings_Red.part
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	Local
		At	Chest
		Part	:Hit_Flash.part
		Part	:Hit_Glow_Red.part
		Part	:Hit_Rings_Red.part
		Lifespan 1
	End
End

#############################################################

End
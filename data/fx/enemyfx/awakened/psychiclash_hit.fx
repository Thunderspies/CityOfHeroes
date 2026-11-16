#############################################################
## PsionicShockwave_Attack.fx
#############################################################

FxInfo
Lifespan 80

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	FS_PsychicLash_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Head
		Part	:Head_Flash.part
		Part	:Head_Shockwave_Small.part
		Part	:Head_Rings_Growing.part
		Part	:Head_Rings_Growing2.part
		Part	:Electricity_Continuing.part
		Lifespan	10
	End

End

#############################################################

End
#############################################################
## SonicBlast_Hit.fx
#############################################################

FxInfo
LifeSpan	200

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Chest
		Part	:Sonic_Hit_Flash_Circle.part
		Part	:Sonic_Hit_Flash.part
		Part	:Sonic_Hit_Rings_Static.part
		Sound PPD_MonitorSphere_SonicHit_01 100 100 .5
		LifeSpan	12

	End


End

Condition
	On 	Time
	Time 	8

	Event
		Type	Start
		At	Chest
		Part	:Sonic_Hit_Flash_Circle.part
		Part	:Sonic_Hit_Flash.part
		Part	:Sonic_Hit_Rings_Static.part

		LifeSpan	12

	End


End


#########################################################################################


End



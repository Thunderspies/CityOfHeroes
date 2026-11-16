#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo
Lifespan 30
#############################################################
## LOOP
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Toxic_Hit_01 100 100 .8
	End

	Event
		Type	Local
		At	Chest
		Part	:Toxic_Splatter.part
		part	:Toxic_Splash.part
		Part	:Toxic_Hit.part
		Lifespan 5
	End

End

#############################################################

End
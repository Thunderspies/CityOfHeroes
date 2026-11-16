#############################################################
## TestTube_Continuing.fx
#############################################################

FxInfo

#############################################################
## LOOP
#############################################################

Condition
	On 	Cycle
	Time 	2

	Event
		Type	Local
		At	Chest
		Part	:Toxic_Splatter.part
		part	:Toxic_Splash_Continuing.part
		Part	:Toxic_Hit.part
		Lifespan 5
	End

End


#############################################################

End
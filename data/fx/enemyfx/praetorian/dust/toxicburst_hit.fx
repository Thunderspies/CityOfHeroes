#############################################################
## Hit_Body_Antimatter.fx
#############################################################

FxInfo
Lifespan 100

#########################################################
####################### AUDIO ###########################
#########################################################


Condition

	On 	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Hamidon_Cripple_Hit_01 100 100 .8
	End

	Event
		EName 	Prime
		Type	start
		At	Chest
		Part	:Explosion_Flash_Quick.part
		Part	:Explosion_Steam_Fill.part
		Part	:Shockwave_Acid.part
		Part	:Toxic_Splash_Initial.part
		Part	:Toxic_Splash_Flash.part
		Lifespan	10
	End
End

#########################################################

Condition

	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy

	End
End

#########################################################

End
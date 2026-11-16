############################################################
##	Fireweapon
###########################################################
FxInfo


Condition
	On 	Time
	Time 	0



	Event
		EName	spawnFire
		Type	Local 
		At	Chest
		Part1	:Inferno.part
		Part2	:InfernorockBits.part		
		Part3	:InfernoExplosion.part
		Sound BirthFX 60 60 .7
	
	End

End

Condition
	On 	Time
	Time 	10

	Event
		EName	spawnFire
		Type	Destroy 
			
	
	End

	End

End

##############################################################


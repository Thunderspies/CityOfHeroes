#########################################################
## Talsorian Gauntlets - Hit Flash (Child FX)
#########################################################

FxInfo

## HIT FLASH #######################################################

Condition
	On Time 
	Time 0
	
	Event
		EName	Pow
		Type 	Local
		At	Chest
		Part1	:Blades_Hit_Flash1.part
		Part5	:Blades_Hit_Star1.part
		##Part3	:Blades_Hit_Embers1.part
		##Part4	:Blades_Hit_EmbersBroad1.part
	End
End

## CLEAN UP #######################################################

Condition
	On Time 
	Time 2
	
	Event
		EName	Pow
		Type 	Destroy
	End
End

#########################################################

End	
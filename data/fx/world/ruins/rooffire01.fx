#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	root
		Part	:RoofFire_flame_A01.part
		Part	:RoofFire_flame_B01.part
		Part	:RoofFire_Smoke01.part
		Part	:RoofFireSpark01.part
		Sound buildingfire_loop 120 120 .77
		
	End
End

End			
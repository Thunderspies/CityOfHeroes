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
		Part	:RoofFire_flame_A.part
		Part	:RoofFire_flame_B.part
		Part	:RoofFire_Smoke.part
		Part	:RoofFireSpark.part
		Sound buildingfire3_loop 180 180 .25
		
	End
End

End			
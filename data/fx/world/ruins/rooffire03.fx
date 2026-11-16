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
		Part	:RoofFire_flame_A03.part
		Part	:RoofFire_flame_B03.part
		Part	:RoofFire_Smoke03.part
		#Part	:RoofFireSpark01.part
		Sound torch1_loop 80 80 .69
		
	End
End

End			
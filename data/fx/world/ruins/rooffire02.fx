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
		Part	:RoofFire_flame_A02.part
		Part	:RoofFire_flame_B02.part
		Part	:RoofFire_Smoke02.part
		#Part	:RoofFireSpark01.part
		Sound torch2_loop 80 80 .49
		
	End
End

End			
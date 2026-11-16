#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On	Time
	Time	5

	Event	
		Type	start
		At	root
		Sound lgfire_loop 90 90 .73
	End
End



Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	start 
		At	root
		#Bhvr	:oilFireOffset.bhvr
		Part	:Largefire_flame_A2.part
		Part	:Largefire_flame_B2.part
		Part	:Largefire_Smoke2.part
		Part	:spark3.part
		Part	:Largefire_Smoke1.part
		
		Part	:Largefire_flame_Flat.part
		Part	:Largefire_flame_Flat2.part
		Sound fireexplo 90 90 .55


		
		
		
	End

End

End			
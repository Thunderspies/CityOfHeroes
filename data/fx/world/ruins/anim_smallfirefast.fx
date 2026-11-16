#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Input  
	InpName	Chest
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Hips
		Part1	WORLD/Ruins/Smallfire_flame_A.part
		Part2	world/Ruins/Smallfire_flame_B.part
		Part3	WORLD/Ruins/Smallfire_flame_Flat.part
		Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/Smallfire_Smoke.part
		Sound Torch2_loop 30 30 .36
		                    BhvrOverride
                                    Behavior
                                    PositionOffset	0 2 0
			End
	End
End

End			
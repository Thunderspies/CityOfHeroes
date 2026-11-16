#########################################################
##	FireRing
##
FxInfo
	


Input  
	InpName	Root
End

Condition
	On 	Time
	Time 	13

	Event
		EName 	Prime
		Type	start 
		At	root
		Part1	WeaponFx/Mediumfire_flame_A.part
		Part2	WeaponFx/Mediumfire_flame_B.part
		Part4	WeaponFx/spark1.part
		Part2	WeaponFx/Mediumfire_flame_C.part
		Sound torch2_loop 35 35 .33
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Ring
		Type	start 
		At	Root
		Part1	powers/firecontrol/BurnFlame01.part
		Part2	powers/firecontrol/Burnringspark.part	
		Sound Torch2_loop 50 50 .5
		
	End

End

Condition
	On 	Time
	Time 	13

	Event
		EName 	ring1
		Type	start
		At	Root
		Part1	powers/firecontrol/BurnFlame02.part
			
	End


End


End						
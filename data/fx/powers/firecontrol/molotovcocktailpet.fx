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
		Part1	WeaponFx/Molotov_flame_A.part
		Part2	WeaponFx/Molotov_flame_B.part
		Part4	WeaponFx/spark1.part
		Part2	WeaponFx/Molotov_flame_C.part
		
	End
End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Ring
		Type	start 
		At	Root
		Part1	powers/firecontrol/MolotovBurnFlame01.part
		Part2	powers/firecontrol/Burnringspark.part	
		Sound torch2_loop 40 40 .33
		
	End

End

Condition
	On 	Time
	Time 	13

	Event
		EName 	ring1
		Type	start
		At	Root
		Part1	powers/firecontrol/MolotovBurnFlame02.part
			
	End


End


End						
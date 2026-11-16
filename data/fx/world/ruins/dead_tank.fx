#########################################################
##	BuildingRuinSmolder
##
FxInfo
	
LifeSpan 30

Input  
	InpName	Origin
End

Input  
	InpName	Root
End

Input  
	InpName	Back
End

Input  
	InpName	Eyes
End

Input  
	InpName	Hair
End

Input  
	InpName	Belt
End

Input  
	InpName	Hips
End

#################################################

Lifespan 300

Condition
	On	Time
	Time	0

	Event
		EName 	Dead_tank
		Type	Start
		At	Origin
		Anim	Dmg_tank_A
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Dead_tank
		AnimPiv Back
		Part1	:MediumGroundSmoke.part 


	End

	Event
		EName 	Prime
		Type	Local 
		At	Dead_tank
		AnimPiv	Hair
		Part1	WORLD/Ruins/Mediumfire_flame_A.part
		Part2	world/Ruins/Mediumfire_flame_B.part
		
		Part3	WORLD/Ruins/Mediumfire_flame_Flat.part
		#Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/mediumfire_Smoke.part
		Sound torch2_loop 35 35 .33
	End

	Event
		EName 	Prime
		Type	Local 
		At	Dead_tank
		AnimPiv	Eyes
		Part1	WORLD/Ruins/Smallfire_flame_A.part
		Part2	world/Ruins/Smallfire_flame_B.part
		Part3	WORLD/Ruins/Smallfire_flame_Flat.part
		Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/Smallfire_Smoke.part
		Sound Torch2_loop 30 30 .36
	End

End

End			
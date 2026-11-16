#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 600								#Overall lifespan for full fire fx



Condition
	On 	Time
	Time 	0							#I believe this is the start frame number

	Event
		EName 	Prime
		Type	Start						#3 choices, Start (stays in locaion of object at start frame), Local, or Posit 
		At	Mystic						#node on skel to attach fire
		Part1	WORLD/Ruins/Mediumfire_flame_A.part
		Part2	world/Ruins/Mediumfire_flame_B.part
		
		Part3	WORLD/Ruins/Mediumfire_flame_Flat.part
		#Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/mediumfire_Smoke.part
		Sound torch2_loop 35 35 .33
	End
End

End			
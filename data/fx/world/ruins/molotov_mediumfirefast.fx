#########################################################
##	SStreetSteeam01
##
FxInfo
	
LifeSpan 600								#Overall lifespan for full fire fx






###### offset node  ###################################################

Condition
	On 	Time
	Time 	0							

	Event
		EName 	Node
		Type	local						
		At	Root
                        BhvrOverride
                                    Behavior
                                    PositionOffset    -1.5 1 25
					End
		
	End
End



###### fire  ############################################################

Condition
	On 	Time
	Time 	0							#I believe this is the start frame number

	Event
		EName 	Prime
		Type	Start						#3 choices, Start (stays in locaion of object at start frame), Local, or Posit 
		At	Node						#node on skel to attach fire
		Part1	WORLD/Ruins/Mediumfire_flame_A.part
		Part2	world/Ruins/Mediumfire_flame_B.part
		
		#Part3	WORLD/Ruins/Mediumfire_flame_Flat.part
		Part4	WORLD/Fire/spark1.part
		Part5	WORLD/Ruins/mediumfire_Smoke.part
		Sound torch2_loop 35 35 .33
	##	Geom	Prop_BoomBox_ground
	End
End

End			
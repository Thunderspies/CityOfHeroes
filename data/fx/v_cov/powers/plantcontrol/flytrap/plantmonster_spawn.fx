#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Mystic
End


Input  
	Inpname	Chest
End


####################################

Lifespan 60

Condition
	On 	Time
	Time 	1

	Event
		EName 	GroundBurst
		Type	Local 
		At	root
		Part1	:Dust.part
		Part1	:Dust.part
		Part3	:pebbles.part
		Part3	:pebbles.part 
		Sound Groundburst 80 80 .96
	End

End


Condition
	On 	Time
	Time 	28

	Event
		
		Type	Local
		At	C_T1_R
		part1	:tentdust.part
		lifespan	15
		pother	C_F2_R
	End
	Event
		
		Type	Local
		At	C_T2_R
		part1	:tentdust.part
		lifespan	15
		pother	C_T1_R
	End
	Event
		
		Type	Local
		At	C_T3_R
		part1	:tentdust.part
		lifespan	15
		pother	C_T2_R
	End
	Event
		
		Type	Local
		At	C_Hind_UlegR
		part1	:tentdust.part
		lifespan	15
		pother	C_T3_R
	End
	Event
		
		Type	Local
		At	C_Hind_LlegR
		part1	:tentdust.part
		lifespan	15
		pother	C_Hind_UlegR
	End
#########
	Event
		
		Type	Local
		At	C_T1_L
		part1	:tentdust.part
		lifespan	15
		pother	C_F2_L
	End
	Event
		
		Type	Local
		At	C_T2_L
		part1	:tentdust.part
		lifespan	15
		pother	C_T1_L
	End
	Event
		
		Type	Local
		At	C_T3_L
		part1	:tentdust.part
		lifespan	15
		pother	C_T2_L
	End
	Event
		
		Type	Local
		At	C_Hind_UlegL
		part1	:tentdust.part
		lifespan	15
		pother	C_T3_L
	End
	Event
		
		Type	Local
		At	C_Hind_LlegL
		part1	:tentdust.part
		lifespan	15
		pother	C_Hind_UlegL
	End

End


#Condition
#	On 	Time
#	Time 	3
#
#	Event
#		EName 	ChestDust
#		Type	Local 
#		At	Root
#		Part1	POWERS\EarthControl\StalagtiteDustHit.part
#		Part2	POWERS\EarthControl\StalagtiteDustHit2.part
#	End
#
#
#End

#Condition
#	On 	Time
#	Time	50
#	
#	Event
#		EName 	ChestDust
#		Type	Destroy
#		
#
#	End
#	
#
#End
#
#Condition
#	On 	Time
#	Time	40
#	
#	Event
#		EName 	GroundBurst
#		Type	Destroy
#		
#
#	End
#	
#
#End


End

#########################################################
##	tech_hologram
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Bhvr	behaviors/world/Lairs/tech/tech_spin.bhvr
		Type	Local 
		At	Origin
		#Geom	tek1_crate_med
		Part1	world/Lairs/tech/tech_hologram_1.part
		Part2	world/Lairs/tech/tech_pillarsmoke1.part
		#Part3	world/Lairs/tech/tech_hologram_2.part
	End

#Condition
#	On 	Time
#	Time 	15
#
#	Event
#		EName 	Prime
#		Bhvr	behaviors/world/Lairs/tech/tech_spin.bhvr
#		Type	Local 
#		At	Origin
#		#Geom	tek1_crate_med
#		Part1	world/Lairs/tech/tech_hologram_1.part
#		#Part2	world/Lairs/tech/tech_lighttentacles_short_2.part
#	End
#End


End


End			
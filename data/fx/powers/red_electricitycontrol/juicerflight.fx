#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Hips
End

Input  
	Inpname	Root
End

########################################################





Condition
	On 	Time
	Time 	0

	Event
		
		Type	Local
		At	Hips
		Part1	:ElectricityCageRing.part
		#Sound	eleccage_loop 100 30 .8
				
	End

End

Condition
	On 	Time
	Time 	15

	Event
		
		Type	Local
		At	Hips
		#Part1	:ElectricityCageArchs2.part
		Part2	:ElectricityCageGlow.part
		#Part3	:ElectricityCageArchs.part
	
	End


	Event
		
		Type	Local
		At	hips
		Geom	Cagearchs2
		bhvr	behaviors/Juicercagespin2.bhvr
	End

End

Condition
	On 	Time
	Time 	16	
	Event
		
		Type	Local
		At	hips
		Geom	Cagearchs
		bhvr	behaviors/Juicercagespin.bhvr
	End


End

End

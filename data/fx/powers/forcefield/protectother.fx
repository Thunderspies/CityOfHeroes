#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Hips
End

########################################################

Condition
	On 	Time
	Time 	0

	
	Event
		EName 	Ball
		Type	posit 
		At	root
		
		Bhvr 	behaviors/ForceBall.bhvr
		
		Geom	ForceBall
				
	End

	Event
		EName 	Ballparts
		Type	local
		At	Ball
		altpiv	1
		
		Part1	:BubbleSphere2.part
				
	End

	Event
		EName 	Ring01
		Type	Local 
		At	Root
		Bhvr 	behaviors/ForceRing02.bhvr
		
		Geom	ForceRing02
						
	End


	Event
		EName 	Ring02
		Type	Local 
		At	Root
		Bhvr 	behaviors/ForceRing01.bhvr
		
		Geom	ForceRing02
						
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Ring03
		Type	Local  
		At	Root
		Bhvr 	behaviors/ForceRing02.bhvr
		
		Geom	ForceRing02
						
	End


	Event
		EName 	Ring04
		Type	Local  
		At	Root
		Bhvr 	behaviors/ForceRing01.bhvr
		
		Geom	ForceRing02
						
	End		

End



End
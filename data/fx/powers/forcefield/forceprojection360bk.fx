#########################################################
##	forceBubble
########################################################
FxInfo

LifeSpan	150

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
		Type	Local 
		At	root
		
		Bhvr 	behaviors/ForceBall.bhvr
		
		Geom	ForceBall
				
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
	Time 	10

	Event
		EName 	Ring
		Type	Local 
		At	Hips
		
		
		Part1	:ForceRing.part
				
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

#################################################################################

Condition
	On 	Time
	Time 	30

	Event
		EName 	RingBlast
		Type	Local 
		At	Hips
				
		Part1	:ForceFlashRing.part
					
	End

	Event
		EName 	RingSmoothGround
		Type	Local 
		At	Root
		#Part1	:ForceRings.part		
		#Part2	:ForceFlashRings.part				
	End

End

Condition
	On 	Time
	Time 	40

	Event
		EName 	Ring
		Type	Destroy				
	End

End

Condition
	On 	Time
	Time 	100

	Event
		EName 	Ring01
		Type	Destroy				
	End

	Event
		EName 	Ring02
		Type	Destroy				
	End

	Event
		EName 	Ring03
		Type	Destroy				
	End

	Event
		EName 	Ring04
		Type	Destroy				
	End

	Event
		EName 	Ball
		Type	Destroy				
	End

End

Condition
	On 	Time
	Time 	70

	Event
		EName 	RingBlast
		Type	Destroy				
	End

	
End


Condition
	On 	Time
	Time 	150

	Event
		EName 	all
		Type	Destroy				
	End

	
End


End
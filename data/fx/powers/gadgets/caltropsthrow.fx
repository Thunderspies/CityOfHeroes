#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	WepR
End

Input  
	Inpname	T_Root
End

Lifespan		200

#################################################################################

Condition
	On	Time
	Time	17

	Event	
		Type	Local
		At	origin
		Sound caltropstoss 60 60 .8
	End
End

Condition
	On 	Time
	Time 	19

	Event
		EName	Prime
		Type	start
		At	WepR
			
		Bhvr	behaviors/CaltropsLobb.bhvr	
		Geom	Caltropsthrow
		
		Magnet	T_Root
		
		#Part1	:webtrail.part
		#Part2	:ForceBallTrail.part

	End

	Event
		EName	Prime2
		Type	start
		At	WepR
			
		Bhvr	behaviors/CaltropsLobb2.bhvr	
		Geom	Caltropsthrow
		
		Magnet	T_Root
		
		#Part1	:webtrail.part
		#Part2	:ForceBallTrail.part

	End

			
End

Condition
	On	PrimeHit
		
		
	Event
		Type    Destroy
		EName 	Prime

	End

End

Condition
	On	Prime2Hit
		
		
	Event
		Type    Destroy
		EName 	Prime2

	End

End

End
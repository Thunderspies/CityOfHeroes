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
	Inpname	Target
End

Lifespan		200

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Prime
		Type	start
		At	WepR
			
		Bhvr	behaviors/NetProjectile.bhvr	
		Geom	netwebbing
		
		Magnet	Target
		LookAt	Target
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

End
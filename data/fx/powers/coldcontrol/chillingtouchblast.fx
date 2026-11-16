#########################################################
##	
##

FxInfo

Input 
	InpName Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Mystic
End

LifeSpan	200

##################################
Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	Mystic
		Bhvr    behaviors/Weapons/Gunfire_tracer.bhvr
		
		Part1	WEAPONFX/Gunfire_pistolTracer.part
		Magnet	Target
		LookAt	Target
		
	End

	Event
		EName 	tracer
		Type	Local 
		At	Mystic

		Part1	:ChillingTouchHands.part
		Part2	:ChillingTouchHandsMiddle.part
	End

End

##################################


Condition
	On 	PrimeHit

	Event
		Ename 	All
		Type	Destroy
	End
End

End				
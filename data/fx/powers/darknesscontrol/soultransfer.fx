#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	T_Chest
End

Input  
	InpName	Chest
End

LifeSpan	200

############################################################

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime
		Type	start 
		At	Target
		
		Bhvr	behaviors/SoulTransfer1.bhvr
		part1	:aFearMist.part
		part2	:aFearMist2.part
		Part3	:aFearTenticles.part
		Part4	:aFearTenticles2.part
		
		
		Magnet	Chest
		Lookat	Chest

	End

End

Condition
	On 	Time
	Time 	130

	Event
		EName 	Prime1
		Type	start 
		At	Target
		
		Bhvr	behaviors/SoulTransfer1.bhvr
		part1	:aFearMist.part
		part2	:aFearMist2.part
		Part3	:aFearTenticles.part
		Part4	:aFearTenticles2.part
		
		Magnet	Chest
		Lookat	Chest

	End

	
End


Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Prime1Hit
	
	Event
		EName 	Prime1
		Type	Destroy

	End



End			
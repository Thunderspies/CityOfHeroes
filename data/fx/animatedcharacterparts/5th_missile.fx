#########################################################
##	5th_missile
##
FxInfo
LifeSpan 90

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	chest
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Start 
		At	Origin
		Bhvr	Behaviors/5th_Missile.bhvr
		Magnet	Target
		LookAt	Target
		Geom	_5th_missile
		Part1	:5thmissile_smoke.part
		Part2	:5thmissile_fire.part
	End

End

Condition
	On	PrimeHit
		
	Event
		Ename	Prime
		Type	Destroy
	End

	
End


End				
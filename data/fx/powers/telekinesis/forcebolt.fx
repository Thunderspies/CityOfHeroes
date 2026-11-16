#########################################################
##	cot_waterfall_med.fx
##
FxInfo
	
LifeSpan	2100

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

###########################################################

Condition

	On 	Time
	Time 	0
	
	Event
		EName 	Prime
		Type	start
		At	WepR
		#Bhvr	behaviors/projectileNOspin.bhvr
		Geom	TeleBolt
		#Magnet	Target
		#LookAt	Target
		#Part1	:ForceMist01.part
		#Part2	:ForceBallTrail.part
	End

	Event
		EName 	Prime2
		Type	start
		At	WepR
		#Bhvr	behaviors/projectileSLIGHTspin.bhvr
		Geom	TeleBoltTail
		#Magnet	Target
		#LookAt	Target
		
	End

End


Condition
	On 	PrimeHit

	Event
		#Type    Destroy
		#EName 	Prime

	End

	Event
		Type    Destroy
		#EName 	Prime2

	End

End

End
		
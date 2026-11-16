#########################################################
##	template

FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	T_Root
End

LifeSpan	35

#########################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Origin
		Sound scrollofruin 70 70 .7
		End
End

Condition
	On 	Time
	Time 	15
	
	Event
		EName 	MaskPantheonDeath
		Type	start 
		At	T_Root
		Part	:Rune1.part
		Part	:QuakeRing.part
		LifeSpan	10		
	End
End

Condition
	On 	Time
	Time 	18
	
	Event
		EName 	MaskPantheonDeath2
		Type	start 
		At	T_Root
		Part	:Rune2.part
		Part	:QuakeRing2.part
				
	End
End

Condition
	On 	Time
	Time 	32
	
	Event
		EName 	ringBurst
		Type	start 
		At	T_Root
		#Part	:Rune2.part
		Part	:QuakeRing.part
		LifeSpan	35		
	End
End

End

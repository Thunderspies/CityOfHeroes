#########################################################

FxInfo

Flags    InheritAlpha
#########################################################

Condition
	
	Event
		EName 	Mask
		Type	Local 
		At	Eyes
		Part	:Rune1.part
//		Part	:Rune2.part
//		Part	:Rune3.part
//		Part	:Rune4.part
		BHVR	:Runespin1.bhvr
	End
End

Condition
	On 	Time
	Time 	5	

	Event
		EName 	Mask1
		Type	Local 
		At	Eyes
		Part	:Rune1.part
		Part	:Rune2.part
		Part	:Rune3.part
		Part	:Rune4.part
		BHVR	:Runespin2.bhvr
		
	End
End

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Mask2
		Type	Local 
		At	Eyes
		Part	:Rune1.part
		Part	:Rune2.part
		Part	:Rune3.part
		Part	:Rune4.part
		BHVR	:Runespin3.bhvr
		
	End

End

End

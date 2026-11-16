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
	Inpname	Head
End

Input  
	Inpname	Eyes
End

Input  
	Inpname	Root
End

Flags    InheritAlpha

#########################################################

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	MaskNode
		Type	Local 
		At	Eyes
		GEOM	Mask_Pivot
		Sound Desirespirit2_loop 68 68 .6
	End

	Event
		EName 	MaskPantheonDesire1
		Type	Local 
		At	Eyes
		Part	:PantheonDesire_Rune1.part
		Part	:PantheonDesire_Rune2.part
		Part	:PantheonDesire_Rune3.part
		Part	:PantheonDesire_Rune4.part
		BHVR	:Runespin1.bhvr
		Sound Deathspirit1_loop 68 68 .76
	End
End

Condition
	On 	Time
	Time 	5	

	Event
		EName 	MaskPantheonDesire2
		Type	Local 
		At	Eyes
		Part	:PantheonDesire_Rune1.part
		Part	:PantheonDesire_Rune2.part
		Part	:PantheonDesire_Rune3.part
		Part	:PantheonDesire_Rune4.part
		BHVR	:Runespin2.bhvr
		
	End
End

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	MaskPantheonDesire3
		Type	Local 
		At	Eyes
		Part	:PantheonDesire_Rune1.part
		Part	:PantheonDesire_Rune2.part
		Part	:PantheonDesire_Rune3.part
		Part	:PantheonDesire_Rune4.part
		BHVR	:Runespin3.bhvr
		
	End

	Event
		EName 	MaskPantheonDesire4
		Type	Local 
		At	MaskNode
		Altpiv	1
		Part	:PantheonDesire_Glow1.part
		Part	:PantheonDesire_Glow2.part
		Part	:PantheonDesire_Cloud1.part

	End

	Event
		EName 	MaskPantheonDesire5
		Type	Local 
		At	MaskNode
		Altpiv	1
		Part	:PantheonDesire_Cloud2.part
	End

	
End

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	MaskNode
		Type	Destroy

	End

	Event		
		Ename	MaskPantheonDesire1
		Type	Destroy

	End
	
	Event		
		Ename	MaskPantheonDesire2
		Type	Destroy

	End
	
	Event		
		Ename	MaskPantheonDesire3
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonDesire4
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonDesire5
		Type	Destroy

	End

End

End

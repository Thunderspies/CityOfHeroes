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
		Sound Deathspirit2_loop 68 68 .6
	End

	Event
		EName 	MaskPantheonDeath
		Type	Local 
		At	Eyes
		Part	:PantheonDeath_Rune1.part
		Part	:PantheonDeath_Rune2.part
		Part	:PantheonDeath_Rune3.part
		Part	:PantheonDeath_Rune4.part
		BHVR	:Runespin1.bhvr
		Sound Deathspirit1_loop 68 68 .76
		
	End
End

Condition
	On 	Time
	Time 	5	

	Event
		EName 	MaskPantheonDeath1
		Type	Local 
		At	Eyes
		Part	:PantheonDeath_Rune1.part
		Part	:PantheonDeath_Rune2.part
		Part	:PantheonDeath_Rune3.part
		Part	:PantheonDeath_Rune4.part
		BHVR	:Runespin2.bhvr
		
	End
End

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	MaskPantheonDeath2
		Type	Local 
		At	Eyes
		Part	:PantheonDeath_Rune1.part
		Part	:PantheonDeath_Rune2.part
		Part	:PantheonDeath_Rune3.part
		Part	:PantheonDeath_Rune4.part
		BHVR	:Runespin3.bhvr
		
	End

	Event
		EName 	MaskPantheonDeath3
		Type	Local 
		At	MaskNode
		Altpiv	1
		Part	:PantheonDeath_Glow1.part
		Part	:PantheonDeath_Glow2.part
		Part	:PantheonDeath_Cloud1.part

	End

	Event
		EName 	MaskPantheonDeath4
		Type	Local 
		At	MaskNode
		Altpiv	1
		Part	:PantheonDeath_Cloud2.part
	End

	
End

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	masknode
		Type	Destroy

	End

	Event		
		Ename	MaskPantheonDeath1
		Type	Destroy

	End
	
	Event		
		Ename	MaskPantheonDeath2
		Type	Destroy

	End
	
	Event		
		Ename	MaskPantheonDeath3
		Type	Destroy

	End

	Event		
		Ename	MaskPantheonDeath4
		Type	Destroy

	End
End

End

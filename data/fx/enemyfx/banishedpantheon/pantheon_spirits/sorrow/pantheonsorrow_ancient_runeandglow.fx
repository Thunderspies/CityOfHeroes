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
		Sound sorrowspirit2_loop 68 68 .6
	End

	Event
		EName 	MaskPantheonSorrow1
		Type	Local 
		At	Eyes
		Part	:PantheonSorrow_Rune1.part
		Part	:PantheonSorrow_Rune2.part
		Part	:PantheonSorrow_Rune3.part
		Part	:PantheonSorrow_Rune4.part
		BHVR	:Runespin1.bhvr
		Sound Deathspirit1_loop 68 68 .76
		
	End
End

Condition
	On 	Time
	Time 	5	

	Event
		EName 	MaskPantheonSorrow2
		Type	Local 
		At	Eyes
		Part	:PantheonSorrow_Rune1.part
		Part	:PantheonSorrow_Rune2.part
		Part	:PantheonSorrow_Rune3.part
		Part	:PantheonSorrow_Rune4.part
		BHVR	:Runespin2.bhvr
		
	End
End

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	MaskPantheonSorrow3
		Type	Local 
		At	Eyes
		Part	:PantheonSorrow_Rune1.part
		Part	:PantheonSorrow_Rune2.part
		Part	:PantheonSorrow_Rune3.part
		Part	:PantheonSorrow_Rune4.part
		BHVR	:Runespin3.bhvr
		
	End

	Event
		EName 	MaskPantheonSorrow4
		Type	Local
		At	MaskNode		
		At	Chest
		Altpiv	1
		Part	:PantheonSorrow_Glow1.part
		Part	:PantheonSorrow_Glow2.part
	End
	
##	Event
##		EName 	MaskPantheonSorrow4
##		Type	Local
##		At	MaskNode		
##		Altpiv	1
##		Part	:PantheonSorrow_Glow1.part
##		Part	:PantheonSorrow_Glow2.part
##		Part	:PantheonSorrow_Cloud1.part
##
##	End
##
##	Event
##		EName 	MaskPantheonSorrow5
##		Type	Local 
##		At	MaskNode
##		Altpiv	1
##		Part	:PantheonSorrow_Cloud2.part
##	End

	
End

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	MaskPantheonSorrow1
		Type	Destroy

	End

	Event		
		Ename	MaskPantheonSorrow2
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonSorrow3
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonSorrow4
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonSorrow5
		Type	Destroy

	End
	
	Event		
		Ename	MaskNode
		Type	Destroy

	End
End

End


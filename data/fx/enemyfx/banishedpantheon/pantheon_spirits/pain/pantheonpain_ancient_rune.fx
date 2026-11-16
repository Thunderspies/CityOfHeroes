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
		Sound Painspirit2_loop 68 68 .35
	End

	Event
		EName 	MaskPantheonPain1
		Type	Local 
		At	Eyes
		Part	:PantheonPain_Rune1.part
		Part	:PantheonPain_Rune2.part
		Part	:PantheonPain_Rune3.part
		Part	:PantheonPain_Rune4.part
		BHVR	:Runespin1.bhvr
		Sound Deathspirit1_loop 68 68 .76
		
	End
End

Condition
	On 	Time
	Time 	5	

	Event
		EName 	MaskPantheonPain2
		Type	Local 
		At	Eyes
		Part	:PantheonPain_Rune1.part
		Part	:PantheonPain_Rune2.part
		Part	:PantheonPain_Rune3.part
		Part	:PantheonPain_Rune4.part
		BHVR	:Runespin2.bhvr
		
	End
End

Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	MaskPantheonPain3
		Type	Local 
		At	Eyes
		Part	:PantheonPain_Rune1.part
		Part	:PantheonPain_Rune2.part
		Part	:PantheonPain_Rune3.part
		Part	:PantheonPain_Rune4.part
		BHVR	:Runespin3.bhvr
		
	End

##	Event
##		EName 	MaskPantheonPain4
##		Type	Local 
##		At	Chest
##		Altpiv	1
##		Part	:PantheonPain_Glow1.part
##		Part	:PantheonPain_Glow2.part
##	End
##
##	Event
##		EName 	MaskPantheonPain4
##		Type	Local 
##		At	MaskNode
##		Altpiv	1
##		Part	:PantheonPain_Glow1.part
##		Part	:PantheonPain_Glow2.part
##		Part	:PantheonPain_Cloud1.part
##	End
##
##	Event
##		EName 	MaskPantheonPain5
##		Type	Local 
##		At	MaskNode
##		Altpiv	1
##		Part	:PantheonPain_Cloud2.part
##	End

	
End

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	MaskNode
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonPain5
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonPain4
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonPain3
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonPain2
		Type	Destroy

	End

	
	Event		
		Ename	MaskPantheonPain1
		Type	Destroy

	End
End

End


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
		EName 	MaskPantheonDeath
		Type	Local 
		At	Root
		Part	:Lichen_01.part
		Part	:Lichen_02.part
		Part	:Lichen_04.part
		Part	:Lichen_05.part
		Part	:Lichen_06.part
		Part	:Lichen_07.part
		Part	:Lichen_Spores.part
		#Sound	lichen1_loop 30 30 .22
	
	End

	Event
		EName 	Prime
		Type	Posit
		At	Root       
		bhvr	:Lichen_Splat.bhvr
		Splat	Mold_02.tga white  
		#Splat	mistring01	woomersphere2
	End
End


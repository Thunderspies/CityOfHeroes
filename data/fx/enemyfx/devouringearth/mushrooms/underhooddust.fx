#########################################################
##	
##
FxInfo

Input  
	InpName	CHEST
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	Head
End


Flags    InheritAlpha


##########################################################


Condition
	On 	Time
	Time 	0

	Event
		Ename Hookup
		Type	Local 
		At	Head
		Geom	MushroomHead	
		Sound air_loop 15 15 .4
		
	End

	Event
		Ename Hookup
		Type	Local 
		At	Hookup	
		Altpiv	1
		Part	:SporeDustHood.part
		Part	:SporeDustHoodMist.part
		Part	:SporeDustHoodNonLocal.part
	End

End


End


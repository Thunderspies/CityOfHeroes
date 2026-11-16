#########################################################
##	SS_launcher_01
##
FxInfo
	
LifeSpan 0

Input  
	InpName	Origin
End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Origin
		Part	:LauncherGlows.part
		Part	:LauncherGlowsBright.part
		Part	:SS_smoke_02.part
	End
End

End			
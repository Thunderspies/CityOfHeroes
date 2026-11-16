#########################################################
##	template

FxInfo

Input  
	Inpname	Origin
End




Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Origin
		Anim	FX_spotlight
		Sound Spotlight_Pivot_Loop 35 35 .20
	End
End



End		
#########################################################
##	template

FxInfo

Input  
	Inpname	LarmL
End



Flags    InheritAlpha


Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	LarmL
		Anim	FX_heavyclaw
	End
End



End		
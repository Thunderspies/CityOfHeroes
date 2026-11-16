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
		Anim	pump_01
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	local
		At	Origin
		Sound pump2_loop 80 80 .76
	End

End

End


FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Root
End

Condition
	Event
		Type	Local
		At	Origin
		Sound SGen2b_loop 90 90 .96
	End
End


Condition
	On	Time
	Time	0

	Event	
		Type	Local
		At	origin
		Sound Bigboomexplo 90 90 .7
		Sound ExploBurst 90 90 .88
	End
End


Condition
	Event
		Type	Local
		At	Origin
		Sound ChainLight2_loop 90 90 .5
	End
End

Condition
	Event
		Type	Local
		At	Origin
		Sound torch2_loop 97 97 1.0
	End
End


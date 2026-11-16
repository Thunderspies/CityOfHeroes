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
		Sound SGen3c_loop 57 57 .7
	End
End

Condition
	On	Time
	Time	3

	Event	
		Type	Local
		At	Origin
		Sound ExploBurst 57 57 .7
	End
End



Condition
	On	Time
	Time	35

	Event	
		Type	Local
		At	Origin
		Sound lightningfield 57 57 .25
	End
End

Condition
	On	Time
	Time	145

	Event	
		Type	Local
		At	Origin
		Sound lightningfield 57 57 .2
	End
End


Condition
	Event
		Type	Local
		At	Origin
		Sound ChainLight2_loop 55 55 .23
	End
End

Condition
	Event
		Type	Local
		At	Origin
		Sound torch2_loop 55 55 .5
	End
End

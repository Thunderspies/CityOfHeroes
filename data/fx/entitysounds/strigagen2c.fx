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
		Sound SGen2c_loop 77 77 1.0
	End
End

Condition
	On	Time
	Time	3

	Event	
		Type	Local
		At	Origin
		Sound Bigboomexplo 77 77 .7
		Sound ExploBurst 77 77 .88
	End
End


Condition
	On	Time
	Time	120

	Event	
		Type	Local
		At	Origin
		Sound Bigboomexplo 77 77 .4
		Sound Grexplo1 77 77 .6
		Sound Grexplo1 77 77 .6
		Sound ExploBurst 77 77 .5
		Sound Grexplo1 77 77 .6
	End
End

Condition
	On	Time
	Time	35

	Event	
		Type	Local
		At	Origin
		Sound lightningfield 77 77 .3
	End
End

Condition
	On	Time
	Time	145

	Event	
		Type	Local
		At	Origin
		Sound lightningfield 77 77 .4
	End
End


Condition
	Event
		Type	Local
		At	Origin
		Sound ChainLight2_loop 77 77 .4
	End
End

Condition
	Event
		Type	Local
		At	Origin
		Sound torch2_loop 77 77 .7
	End
End

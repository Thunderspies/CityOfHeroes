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
		Sound GenExplo1 90 90 .96
	End
End



Condition
	Event
		Type	Local
		At	Origin
		#Sound	Atomicblast 90 50 .96
	End
End

Condition
	Event
		Type	Local
		At	Origin
		Sound torch2_loop 97 97 .8
	End
End




FxInfo

Flags	InheritAnimScale

Lifespan 125


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	WepR
		Sound rushghost 50 50 .3
		
	End

End


Condition
	On 	Time
	Time 	12

	Event
		Type	Local 
		At	WepR
		Sound stungunout 50 50 .33
		
	End

End

Condition
	On	Time
	Time	12

	Event
		EName	NodeSpin
		Type	Local
		At	WepL

		bhvr	V_COV\Powers\Paramilitary\Paramilitary_B_BoostSerumPos.bhvr
		
		Geom	GEO_BoostSerum
	End
End

Condition
	On	Time
	Time	60
	
	Event
		EName	Glow
		Type	Local
		At	Root
		Sound syringe5 50 50 .4
	End
End

End


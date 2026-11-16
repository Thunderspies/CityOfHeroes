#############################################################
## FX System Name
#############################################################

FxInfo

#############################################################

Condition
	On	Time	
	Time 	0

	Event
		Type 	Local
		At	Origin
		Sound leaderglow3 80.0 80.0 .55
		Sound glow2b_loop 80.0 80.0 .38
	End
End

#############################################################

Condition
	On	Time
	Time	0
	
	Event
		Type	Local 
		At	Root
		Geom	FX_RiktiPortalStabilizer_Shield
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Root
		part	:Forcefield\Ring_Base1.part
		part	:Forcefield\Ring_Base2.part
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		Type	Local 
		At	Root
		part	:Forcefield\Ring_Symbols1.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		Type	Local 
		At	Root
		part	:Forcefield\Ring_Symbols2.part
		Lifespan	1
	End
End

Condition
	On	Cycle
	Time	5
	Chance	0.125

	Event
		Type	Local 
		At	Root
		part	:Forcefield\Ring_Symbols3.part
		Lifespan	1
	End
End

#############################################################

End				
#########################################################
##	
##
FxInfo
	
Name 	BodyChill
LifeSpan 0

Input  
	InpName	HIPS
End

Input  
	InpName	CHEST
End

Input  
	InpName	HEAD
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	ULEGL
End

Input  
	InpName	ULEGR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End


Condition
	On 	Time
	Time 	0

	Event
		EName	IceHip
		Type	Local 
		At	HIPS
		Part1 powers/coldcontrol/bodychillbig.part
	End

	Event
		Type	Local 
		At	CHEST
		Part1 powers/coldcontrol/bodychillbig.part
	End
End


Condition
	On 	Time
	Time 	10

	Event
		Type	Local 
		At	HEAD
		Part1 powers/coldcontrol/bodychillbig.part
	End

	Event
		Type	Local 
		At	UARMR
		Part1 powers/coldcontrol/bodychillbig.part
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	Local 
		At	UARML
		Part1 powers/coldcontrol/bodychillbig.part
	End

	Event
		Type	Local 
		At	LARML
		Part1 powers/coldcontrol/bodychillbig.part
	End

End

Condition
	On 	Time
	Time 	30

	Event
		Type	Local 
		At	LARMR
		Part1 powers/coldcontrol/bodychillbig.part
	End

	Event
		Type	Local 
		At	ULEGL
		Part1 powers/coldcontrol/bodychillbig.part
	End
End

Condition
	On 	Time
	Time 	40


	Event
		Type	Local 
		At	ULEGR
		Part1 powers/coldcontrol/bodychillbig.part
	End


	Event
		EName	MYLEGL
		Type	Local 
		At	LLEGL
		Part1 powers/coldcontrol/bodychillbig.part
	End

End

Condition
	On 	Time
	Time 	50

	Event
		EName	MYLEGR
		Type	Local 
		At	LLEGR
		Part1 powers/coldcontrol/bodychillbig.part
	End

End

Condition
	On 	Time
	Time 	10
	

	Event	

		Type	Start
		At	CHEST
		Part1	powers/coldcontrol/iceboltsnowexp.part
	End


End


End			
#########################################################
##	template

FxInfo

Input  
	Inpname	F1_L
End

Input  
	Inpname	F1_R
End

Input  
	Inpname	F2_L
End

Input  
	Inpname	F2_R
End



Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	F1_L
		Geom	GEO_ring__Reclamring
	End

	Event
		Type	Local
		At	F2_L
		Geom	GEO_ring__Reclamring
	End

	Event
		Type	Local
		At	F1_R
		Geom	GEO_ring__Reclamring
	End

	Event
		Type	Local
		At	F2_R
		Geom	GEO_ring__Reclamring
	End

End



End		
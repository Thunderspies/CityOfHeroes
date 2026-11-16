#########################################################
##	Template

FxInfo


Condition
	On 	Time
	Time 	0

#######  OFFSETS
	Event
		EName	Offset_01
		Type	Local
		At	Head
		Bhvr	:Offset_01.bhvr

	End
End
Condition
	On 	Cycle
	Time 	85

######## GEOMETRY

	Event
		EName	Offset
		Type	Local
		At	Offset_01
		Geom	Wailer_Eyelid
		Lifespan 4
	End

End

Condition
	On 	Cycle
	Time 	185

######## GEOMETRY

	Event
		EName	Offset
		Type	Local
		At	Offset_01
		Geom	Wailer_Eyelid
		Lifespan 4
	End

End

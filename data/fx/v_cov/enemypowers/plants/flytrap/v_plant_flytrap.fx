#########################################################
##	template

FxInfo


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound tree_emerge 70 70 .7
	End
End

Condition
	On	Time
	Time	33

	Event
		Type	Local
		At	Origin
		Sound creatureroar1 70 70 .82
	End
End



Condition
	On	Time
	Time	0

	Event	
		ENAME	Start
		Type	Local
		At	Hips
		part	:Flytrap_01.part
	End	
End

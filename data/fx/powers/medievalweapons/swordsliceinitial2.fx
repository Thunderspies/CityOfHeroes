#########################################################
##	template

FxInfo

Input
	Inpname Origin
End

Input
	Inpname Mystic
End

LifeSpan	50


Condition
	On 	Time
	Time 	12

	Event
		Type	local
		At	Origin
		Sound swordshing 100 100 .7
	End

End

Condition
	On 	Time
	Time 	45

	Event
		EName	Streak
		Type	Start
		At	Mystic
		Geom	swoosh
		bhvr	behaviors/Medieval1.bhvr
		Sound Whooshbig3 100.0 100.0 .7
		Lifespan 1
	End

End

	
End	

	
#############################################################
## HEADER
#############################################################

FXInfo

#############################################################

Condition
	On	Cycle
	Time	1
	Chance	0.5

	Event
		Type	start
		At	Origin
		BhvrOverride
			PyrRotateJitter	180 180 180
		End
		Part1	ENEMYFX\Rikti\RiktiRandomEllipse.part
		Lifespan 5
	End
End

#############################################################

End
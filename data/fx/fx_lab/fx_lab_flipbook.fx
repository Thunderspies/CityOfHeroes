#########################################################
## IncarnatePower  - FlipbookTest
#########################################################

FxInfo
LifeSpan 300

########################################################

Input
	Inpname	Hips
End

########################################################



## POWER FX #######################################################


Condition
	On	Time
	Time	1

	Event
		EName	AOERing
		Type	start
		At	Root
		BhvrOverride
          		PositionOffset 0.0 5 0.0
         	End
		Part2	:fx_lab_flipbook.part
#		Lifespan	20
	End

End
End
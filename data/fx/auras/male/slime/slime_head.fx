#########################################################
## Slime Aura Head
#########################################################

FxInfo

Flags DontSuppress
########################################################



## SOUND FX #######################################################

Condition
	On	Time
	Time	0

	Event
		Type	start
		At	Root
#		Sound explohit 80 80 0.77
	End
End

## POWER FX #######################################################



Condition
	On	Time
	Time	1

	Event
		EName	HeadSlime
		Type	PositOnly
		At	Face
		BhvrOverride
#          		PositionOffset 1.0 1.5 1.0
         	End
		Part2	:Slime_Idle.part

	End

End


Condition
	On	Time
	Time	1

	Event
		EName	HeadSlime
		Type	PositOnly
		At	Face
		BhvrOverride
          		PositionOffset  0 0 0
         	End

		Part2	:Slime_Face.part

	End

End

End
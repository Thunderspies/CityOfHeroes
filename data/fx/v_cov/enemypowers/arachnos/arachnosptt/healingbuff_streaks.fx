#########################################################
## Arachnos PTT - Buff (Healing)
#########################################################

FxInfo

LifeSpan	30

#########################################################

## Streak ##
Condition
	On 	Time
	Time 	0

	Event
		EName 	Streak
		Type	Local 
		At	DummyNodeOffset

		Part1	:Streakers_Green.part

		Magnet	Target
		LookAt	Target

	End

End


#########################################################

#Condition
#	On	Prime1Hit
#		
#	Event
#		Ename	Prime1
#		Type	Destroy
#	End
#
#End

End
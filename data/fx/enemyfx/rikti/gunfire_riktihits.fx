#########################################################
##	RadiationAura
FxInfo


Input  
	InpName	chest
End

##################################
	
Condition
	On	Time
	Time	0

	Event
		EName	Ring
		Type	local
		At	chest
		geom	centerdummy
		bhvr	behaviors/spin4.bhvr
		Sound rikitgunhit 80 80 .88
	End
End


Condition
	On	Time
	Time	2

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
	End


End


Condition
	On	Time
	Time	4


	Event
		EName	explosion57
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
		Part2	:RiktiEnergyBubblingPurple.part
		Part3	:RiktiEnergyBubbling.part
	End

End

Condition
	On	Time
	Time	8


	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
	End

End


Condition
	On	Time
	Time	12


	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
	End

End


Condition
	On	Time
	Time	10


	Event
		EName	explosion
		Type	start
		At	chest
		Part1	:RiktiRandomEllipse.part
		
	End

End

Condition
	On	Time
	Time	10


	Event
		EName	spit
		Type	start
		At	Ring
		altpiv	1
		
		Part2	:RiktiPortalSpecks2.part
	End

End

Condition
	On	Time
	Time	5

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part

	End

End


Condition
	On	Time
	Time	11

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
		
	End


End


Condition
	On	Time
	Time	13

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
	End


End


Condition
	On	Time
	Time	6

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
	End

End


Condition
	On	Time
	Time	11

	Event
		EName	explosion
		Type	start
		At	Ring
		altpiv	1
		Part1	:RiktiRandomEllipse.part
	End


End

Condition
	On	Time
	Time	15

	Event
		EName	spit
		Type	destroy

	End

	
End


Condition
	On	Time
	Time	17

	
	Event
		EName	explosion57
		Type	destroy

	End

End

Condition
	On	Time
	Time	40

	Event
		EName	all
		Type	destroy

	End

End

End				